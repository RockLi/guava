/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_request.h"
#include "guava_string.h"
#include "guava_conn.h"
#include "guava_server.h"
#include "guava_response.h"
#include "guava_handler.h"
#include "guava_router/guava_router.h"
#include "guava_mime_type.h"
#include "guava_module.h"
#include "guava_session/guava_session.h"
#include "guava_cookie.h"

static guava_request_method_t guava_request_methods[] = {
  {0, "DELETE"},
  {1, "GET"},
  {2, "HEAD"},
  {3, "POST"},
  {4, "PUT"},
  /* pathological */
  {5, "CONNECT"},
  {6, "OPTIONS"},
  {7, "TRACE"},
  /* webdav */                      \
  {8, "COPY"},
  {9, "LOCK"},
  {10, "MKCOL"},
  {11, "MOVE"},
  {12, "PROPFIND"},
  {13, "PROPPATCH"},
  {14, "SEARCH"},
  {15, "UNLOCK"},
  /* subversion */
  {16, "REPORT"},
  {17, "MKACTIVITY"},
  {18, "CHECKOUT"},
  {19, "MERGE"},
  /* upnp */
  {20, "MSEARCH"},
  {21, "NOTIFY"},
  {22, "SUBSCRIBE"},
  {23, "UNSUBSCRIBE"},
  /* RFC-5789 */
  {24, "PATCH"},
  {25, "PURGE"}
};

guava_request_t *guava_request_new(void) {
  guava_request_t *req = (guava_request_t *)calloc(1, sizeof(guava_request_t));
  if (!req) {
    return NULL;
  }

  req->url = NULL;
  req->path = NULL;
  req->host = NULL;
  req->body = NULL;

  req->headers = PyDict_New();
  if (!req->headers) {
    free(req);
    return NULL;
  }

  req->major = 1;
  req->minor = 1;
  req->method = HTTP_GET;

  req->GET = PyDict_New();
  if (!req->GET) {
    Py_DECREF(req->headers);
    free(req);
    return NULL;
  }

  req->POST = PyDict_New();
  if (!req->POST) {
    Py_DECREF(req->headers);
    Py_DECREF(req->GET);
    free(req);
    return NULL;
  }

  req->COOKIES = PyDict_New();
  if (!req->COOKIES) {
    Py_DECREF(req->headers);
    Py_DECREF(req->GET);
    Py_DECREF(req->POST);
    free(req);
    return NULL;
  }

  return req;
}

void guava_request_free(guava_request_t *req) {
  if (req->url) {
    guava_string_free(req->url);
    req->url = NULL;
  }

  if (req->path) {
    guava_string_free(req->path);
    req->path = NULL;
  }

  if (req->host) {
    guava_string_free(req->host);
    req->host = NULL;
  }

  if (req->body) {
    guava_string_free(req->body);
    req->body = NULL;
  }

  if (req->headers) {
    Py_DECREF(req->headers);
    req->headers = NULL;
  }

  if (req->GET) {
    Py_DECREF(req->GET);
    req->GET = NULL;
  }

  if (req->POST) {
    Py_DECREF(req->POST);
    req->POST = NULL;
  }

  if (req->COOKIES) {
    Py_DECREF(req->COOKIES);
    req->COOKIES = NULL;
  }

  free(req);
}

int8_t guava_request_get_method(const char *s) {
  for (size_t i = 0; i < sizeof(guava_request_methods) / sizeof(guava_request_methods[0]); ++i) {
    if (strncmp(guava_request_methods[i].name, s, strlen(s)) == 0) {
      return guava_request_methods[i].code;
    }
  }

  return -1;
}

int guava_request_on_message_begin(http_parser *parser) {
  guava_conn_t *conn = (guava_conn_t *)parser->data;

  Request *request = (Request *)PyObject_New(Request, &RequestType);

  request->req = guava_request_new();
  conn->request = (PyObject *)request;
  Py_INCREF(request);

  if (!conn->request) {
    return -1;
  }

  return 0;
}

void guava_request_extract_from_url(guava_request_t *req) {
  if (!req->url) {
    return;
  }

  char *ptr = strchr(req->url, '?');

  if (!ptr) {
    req->path = guava_string_new(req->url);
  } else {
    req->path = guava_string_new_size(req->url, ptr - (char *)req->url);
    ptr += 1;
    if (ptr != NULL) {
      char *and_ptr = strchr(ptr, '&');
      char *equal_ptr = NULL;

      while (ptr) {
        if (!and_ptr) {
          equal_ptr = strchr(ptr, '=');
          if (equal_ptr) {
            PyObject *key = PyString_FromStringAndSize(ptr, equal_ptr-ptr);
            PyObject *value = PyString_FromString(equal_ptr+1);
            PyDict_SetItem(req->GET, key, value);
          }
          break;
        } else {
          guava_string_t v = guava_string_new_size(ptr, and_ptr-ptr);
          equal_ptr = strchr(ptr, '=');
          if (equal_ptr) {
            PyObject *key = PyString_FromStringAndSize(ptr, equal_ptr-ptr);
            PyObject *value = PyString_FromStringAndSize(equal_ptr+1, and_ptr-equal_ptr-1);
            PyDict_SetItem(req->GET, key, value);
          }
          guava_string_free(v);
          ptr = and_ptr + 1;
          if (!ptr) {
            break;
          }
          and_ptr = strchr(ptr, '&');
        }
      }
    }
  }
}

int guava_request_on_url(http_parser *parser, const char *buf, size_t len) {
  guava_conn_t *conn = (guava_conn_t *)parser->data;
  Request *request = (Request *)conn->request;
  request->req->url = guava_string_new_size(buf, len);

  guava_request_extract_from_url(request->req);
  return 0;
}

int guava_request_on_header_field(http_parser *parser, const char *buf, size_t len) {
  guava_conn_t *conn = (guava_conn_t *)parser->data;

  if (conn->auxiliary_last_was_header) {
    return -1;
  }

  conn->auxiliary_current_header = guava_string_new_size(buf, len);
  conn->auxiliary_last_was_header = 1;
  return 0;
}

int guava_request_on_header_value(http_parser *parser, const char *buf, size_t len) {
  guava_conn_t *conn = (guava_conn_t *)parser->data;
  Request *request = (Request *)conn->request;

  if (conn->auxiliary_last_was_header && conn->auxiliary_current_header) {
    PyObject *v = PyString_FromStringAndSize(buf, len);
    PyDict_SetItemString(request->req->headers, conn->auxiliary_current_header, v);
    Py_DECREF(v);
    guava_string_free(conn->auxiliary_current_header);
    conn->auxiliary_current_header = NULL;
  }

  conn->auxiliary_last_was_header = 0;

  return 0;
}

int guava_request_on_headers_complete(http_parser *parser) {
  guava_conn_t *conn = (guava_conn_t *)parser->data;
  Request *request = (Request *)conn->request;

  request->req->major = parser->http_major;
  request->req->minor = parser->http_minor;
  request->req->keep_alive = http_should_keep_alive(parser);
  request->req->method = parser->method;

  conn->keep_alive = request->req->keep_alive;

  conn->auxiliary_current_header = NULL;
  conn->auxiliary_last_was_header = 0;

  PyObject *host = PyDict_GetItemString(request->req->headers, "Host");
  if (host) {
    request->req->host = guava_string_new(PyString_AsString(host));
  }

  PyObject *cookie = PyDict_GetItemString(request->req->headers, "Cookie");
  if (cookie) {
    char *p = PyString_AsString(cookie);
    char **data = (char **)&p;
    Cookie *c = NULL;

    while ((c = (Cookie *)guava_cookie_parse(data))) {
      PyDict_SetItemString(request->req->COOKIES, c->data.name, (PyObject *)c);
    }
  }

  return 0;
}

int guava_request_on_body(http_parser *parser, const char *buf, size_t len) {
  guava_conn_t *conn = (guava_conn_t *)parser->data;
  Request *request = (Request *)conn->request;
  if (len) {
    request->req->body = guava_string_append_raw_size(request->req->body, buf, len);
  }

  return 0;
}

void on_write(uv_write_t *req, int status) {
  guava_response_t *resp = (guava_response_t *)req->data;
  guava_conn_t *conn = resp->conn;
  guava_response_free(resp);
  if (!uv_is_closing((uv_handle_t *)&conn->stream)) {
    if (!conn->keep_alive) {
      uv_close((uv_handle_t *)&conn->stream, guava_server_on_close);
    }
  }
}

void on_sendfile(uv_fs_t *req) {
  if (req->result < 0) {
    fprintf(stderr, "sendfile error: %s\n", uv_strerror((int)req->result));
  }
}

int guava_request_on_message_complete(http_parser *parser) {
  guava_conn_t *conn = (guava_conn_t *)parser->data;
  Request *request = (Request *)conn->request;
  guava_server_t *server = conn->server;

  Router *router = NULL;
  Handler *handler = NULL;

  Py_ssize_t nrouters = PyList_Size(server->routers);

  router = (Router *)guava_router_get_best_matched_router((PyObject *)server->routers, (PyObject *)request);
  if (router) {
    handler = (Handler *)PyObject_CallMethod((PyObject *)router, "route", "(O)", request);
    handler->handler->router = router->router;
  }

  for (Py_ssize_t i = 0; i < nrouters; ++i) {
    router = (Router *)PyList_GetItem(server->routers, i);
    if (router->router->type != GUAVA_ROUTER_CUSTOM) {
      /*
       * Cause we already try to get the best matched router
       * But we still need to pass the custome router in case use defined some speciall routes
       */
      continue;
    }
    Handler *h = (Handler *)PyObject_CallMethod((PyObject *)router, "route", "(O)", request);
    if ((PyObject *)h != Py_None) {
      handler = h;
      handler->handler->router = router->router;
      break;
    }
  }

  guava_response_t *resp = guava_response_new();
  guava_response_set_conn(resp, conn);

  do {
    if (!guava_handler_is_valid(handler->handler) || handler->handler->flags & GUAVA_HANDLER_404) {
      guava_response_404(resp, NULL);
      guava_response_send(resp, on_write);
      break;
    }

    if (handler->handler->flags & GUAVA_HANDLER_REDIRECT) {
      PyObject *location = PyTuple_GetItem(handler->handler->args, 0);
      guava_response_302(resp, PyString_AsString(location));
      guava_response_send(resp, on_write);
      break;
    }

    if (handler->handler->router->type == GUAVA_ROUTER_STATIC) {
      guava_handler_static(handler->handler->router, conn, ((Request *)conn->request)->req, resp, on_write, on_sendfile);
      break;
    }

    PyObject *module_name = NULL;
    if (guava_string_equal_raw(handler->handler->package, ".")) {
      module_name = PyString_FromString(handler->handler->module);
    } else {
      module_name = PyString_FromFormat("%s.%s", handler->handler->package, handler->handler->module);
    }

    PyObject *module = PyImport_Import(module_name);
    Py_DECREF(module_name);

    if (!module) {
      fprintf(stderr, "no module named: %s\n", PyString_AsString(module_name));
      guava_response_500(resp, NULL);
      guava_response_send(resp, on_write);
      break;
    }

    PyObject *cls_name = PyString_FromString(handler->handler->cls);
    PyObject *cls = PyObject_GetAttr(module, cls_name);

    Py_DECREF(module);
    Py_DECREF(cls_name);

    if (!cls) {
      fprintf(stderr, "no cls named: %s\n", PyString_AsString(cls_name));
      guava_response_500(resp, NULL);
      guava_response_send(resp, on_write);
      break;
    }

    Controller *c = (Controller *)PyObject_CallObject(cls, NULL);

    if (!PyObject_TypeCheck(c, &ControllerType)) {
      fprintf(stderr, "You controller class must inherit from guava.controller.Controller\n");
      guava_response_500(resp, NULL);
      guava_response_send(resp, on_write);
      break;
    }

    Py_DECREF(cls);
    c->resp = resp;
    c->req = ((Request *)conn->request)->req;
    c->router = handler->handler->router;

    if (!PyObject_CallMethod((PyObject *)c, handler->handler->action, NULL)) {
      Py_DECREF(c);
      PyErr_Print();
      fprintf(stderr, "failed to execute action: %s\n", handler->handler->action);
      guava_response_500(resp, NULL);
      guava_response_send(resp, on_write);
      break;
    }

    if (handler->handler->router->session_store && c->SESSION) {
      guava_request_t *r = ((Request *)conn->request)->req;

      PyObject *sid_cookie = PyDict_GetItemString(r->COOKIES, handler->handler->router->session_store->name);

      guava_session_id_t sid = NULL;

      if (sid) {
        sid = guava_string_new(((Cookie *)sid_cookie)->data.value);
      } else {
        sid = guava_session_new_id();
      }

      guava_session_set(handler->handler->router->session_store, sid, c->SESSION);
    }

    Py_DECREF(c);

    guava_response_send(resp, on_write);
  } while(0);

  Py_XDECREF(handler);
  Py_XDECREF(conn->request);

  return 0;
}
