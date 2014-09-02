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
#include "guava_memory.h"
#include "guava_url.h"

#include <assert.h>

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
  guava_request_t *req = (guava_request_t *)guava_calloc(1, sizeof(guava_request_t));
  if (!req) {
    return NULL;
  }

  req->url = NULL;
  req->path = NULL;
  req->host = NULL;
  req->body = NULL;

  req->HEADERS = PyDict_New();
  if (!req->HEADERS) {
    guava_free(req);
    return NULL;
  }

  req->major = 1;
  req->minor = 1;
  req->method = HTTP_GET;

  req->GET = PyDict_New();
  if (!req->GET) {
    Py_DECREF(req->HEADERS);
    guava_free(req);
    return NULL;
  }

  req->POST = PyDict_New();
  if (!req->POST) {
    Py_DECREF(req->HEADERS);
    Py_DECREF(req->GET);
    guava_free(req);
    return NULL;
  }

  req->COOKIES = PyDict_New();
  if (!req->COOKIES) {
    Py_DECREF(req->HEADERS);
    Py_DECREF(req->GET);
    Py_DECREF(req->POST);
    guava_free(req);
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

  if (req->HEADERS) {
    Py_DECREF(req->HEADERS);
    req->HEADERS = NULL;
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

  guava_free(req);
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
    PyDict_SetItemString(request->req->HEADERS, conn->auxiliary_current_header, v);
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

  PyObject *host = PyDict_GetItemString(request->req->HEADERS, "Host");
  if (host) {
    request->req->host = guava_string_new(PyString_AsString(host));
  }

  PyObject *cookie = PyDict_GetItemString(request->req->HEADERS, "Cookie");
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

    PyObject *content_type = PyDict_GetItemString(request->req->HEADERS, "Content-Type");
    if (content_type) {
      char *form_data = request->req->body;
      char **p = &form_data;
      if (strcmp(PyString_AsString(content_type), "application/x-www-form-urlencoded") == 0) {
        guava_string_t name = NULL;
        guava_string_t value = NULL;

        while (guava_request_parse_form_data(p, &name, &value)) {
          PyObject *s = PyString_FromString(value);
          PyDict_SetItemString(request->req->POST, name, s);
          Py_DECREF(s);

          name = NULL;
          value = NULL;
        }

        if (name) {
          guava_string_free(name);
        }
        if (value) {
          guava_string_free(value);
        }

      }
    }
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

  guava_response_t *resp = guava_response_new();
  guava_response_set_conn(resp, conn);

  Py_ssize_t nrouters = PyList_Size(server->routers);

  do {
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

    assert(handler != NULL);
    assert(handler->handler != NULL);

    if (!handler ||
        !handler->handler ||
        !guava_handler_is_valid(handler->handler) ||
        handler->handler->flags & GUAVA_HANDLER_404) {
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
      if (PyErr_Occurred()) {
        PyErr_Print();
      }
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
      if (PyErr_Occurred()) {
        PyErr_Print();
      }
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

    PyObject *r = NULL;

    do {

      r = PyObject_CallMethod((PyObject *)c, "before_action", NULL);
      if (!r) {
        goto process_500;
      }
      /* @todo: check we could stop here in advance */

      if (handler->handler->args) {
        r = PyObject_CallMethod((PyObject *)c, handler->handler->action, "O", handler->handler->args);
      } else {
        r = PyObject_CallMethod((PyObject *)c, handler->handler->action, NULL);
      }

      if (!r) {
        goto process_500;
      }

      /* @todo: check whether we could stop here */

      r = PyObject_CallMethod((PyObject *)c, "after_action", NULL);
      if (!r) {
        goto process_500;
      }

    } while (0);

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

    goto send;

  process_500:
    if (PyErr_Occurred()) {
      PyErr_Print();
    }
    guava_response_500(resp, NULL);

  send:
    Py_DECREF(c);
    guava_response_send(resp, on_write);
  } while(0);

  Py_XDECREF(handler);
  Py_XDECREF(conn->request);

  return 0;
}

char *guava_request_parse_form_data(char **data, guava_string_t *name, guava_string_t *value) {
  if (!data || *data == '\0') {
    return NULL;
  }

  char *p = *data;

  char *name_start = NULL;
  char *name_end = NULL;
  char *value_start = NULL;
  char *value_end = NULL;

  while(isspace(*p)) {
    ++p;
  }

  do {
    if (*p == '\0') {
      break;
    }

    name_start = p;

    while (*p != '=') {
      ++p;
    }

    if (*p != '=') {
      break;
    }

    name_end = p++;

    if (*p == '\0') {
      break;
    }

    value_start = p;

    while (*p != '&' && *p != '\0') {
      ++p;
    }

    value_end = p;

    if (*p == '&') {
      ++p;
    }

    *data = p;
  } while(0);

  if (!name_start || !name_end || !value_start || !value_end) {
    return NULL;
  }

  *name = guava_string_new_size(name_start, name_end - name_start);
  guava_string_t v = guava_string_new_size(value_start, value_end - value_start);
  *value = guava_url_decode(v);
  guava_string_free(v);

  return *data;
}
