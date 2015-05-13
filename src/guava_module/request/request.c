/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_request.h"
#include "guava_module.h"
#include "guava_string.h"
#include "guava_memory.h"

static PyObject *Request_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  Request *self;

  self = (Request *)type->tp_alloc(type, 0);
  if (self) {
    self->req = guava_request_new();
  }

  return (PyObject *)self;
}

static void Request_dealloc(Request *self) {
  guava_request_free(self->req);

  self->ob_type->tp_free((PyObject *)self);
}

static int Request_init(Request *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"url", "path", "host", "method", "body", "HEADERS", "GET", "POST", "COOKIES", NULL};

  char *url = NULL;
  char *path = NULL;
  char *host = NULL;
  char *method = NULL;
  char *body = NULL;
  PyObject *HEADERS = NULL;
  PyObject *GET = NULL;
  PyObject *POST = NULL;
  PyObject *COOKIES = NULL;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "|sssssOOOO",
                                   kwlist,
                                   &url,
                                   &path,
                                   &host,
                                   &method,
                                   &body,
                                   &HEADERS,
                                   &GET,
                                   &POST,
                                   &COOKIES
                                   )) {

    return -1;
  }

  if (url) {
    self->req->url = guava_string_new(url);
    if (!path) {
        guava_request_extract_from_url(self->req);
    }
  }

  if (path) {
    self->req->path = guava_string_new(path);
  }

  if (host) {
    self->req->host = guava_string_new(host);
  }

  if (method) {
    self->req->method = guava_request_get_method(method);
  }

  if (body) {
    self->req->body = guava_string_new(body);
  }

  if (HEADERS) {
    Py_INCREF(HEADERS);
    self->req->HEADERS = HEADERS;
  }

  if (GET) {
    Py_INCREF(GET);
    self->req->GET = GET;
  }

  if (POST) {
    Py_INCREF(POST);
    self->req->POST = POST;
  }

  return 0;
}

static PyObject *Request_get_method(Request *self, void *closure) {
  guava_request_t *req = self->req;

  return PyString_FromString(http_method_str(req->method));
}

static int Request_set_method(Request *self, PyObject *value, void *closure) {
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the method property");

  guava_request_t *req = self->req;

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "the method attribute must be string");
    return -1;
  }

  req->method = guava_request_get_method(PyString_AsString(value));

  return 0;
}

static PyObject *Request_get_url(Request *self, void *closure) {
  guava_request_t *req = self->req;

  if (req->url) {
    return PyString_FromString(req->url);
  }

  Py_RETURN_NONE;
}

static int Request_set_url(Request *self, PyObject *value, void *closure) {
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the url property");





  return 0;
}


static PyObject *Request_get_host(Request *self, void *closure) {
  guava_request_t *req = self->req;

  if (req->host) {
    return PyString_FromString(req->host);
  }

  Py_RETURN_NONE;
}

static PyObject *Request_get_body(Request *self, void *closure) {
  guava_request_t *req = self->req;

  if (req->body) {
    return PyString_FromString(req->body);
  }

  Py_RETURN_NONE;
}

static PyObject *Request_get_GET(Request *self, void *closure) {
  guava_request_t *req = self->req;
  if (req->GET) {
    Py_INCREF(req->GET);
    return req->GET;
  }

  Py_RETURN_NONE;
}

static PyObject *Request_get_POST(Request *self, void *closure) {
  guava_request_t *req = self->req;
  if (req->POST) {
    Py_INCREF(req->POST);
    return req->POST;
  }

  Py_RETURN_NONE;
}

static PyObject *Request_get_COOKIES(Request *self, void *closure) {
  guava_request_t *req = self->req;
  if (req->COOKIES) {
    Py_INCREF(req->COOKIES);
    return req->COOKIES;
  }

  Py_RETURN_NONE;
}

static PyObject *Request_get_HEADERS(Request *self, void *closure) {
  guava_request_t *req = self->req;
  Py_INCREF(req->HEADERS);

  return req->HEADERS;
}

static PyObject *Request_get_path(Request *self, void *closure) {
  guava_request_t *req = self->req;

  if (req->path) {
    return PyString_FromString(req->path);
  }

  Py_RETURN_NONE;
}

static PyGetSetDef Request_getseter[] = {
  /* {"method", (getter)Request_get_method, (setter)Request_set_method, "method", NULL}, */
  /* {"url", (getter)Request_get_url, (setter)Request_set_url, "url", NULL}, */
  /* {"path", (getter)Request_get_path, (setter)Request_set_path, "path", NULL}, */
  /* {"host", (getter)Request_get_host, (setter)Request_set_host, "host", NULL}, */
  /* {"body", (getter)Request_get_body, (setter)Request_set_body, "body", NULL}, */
  /* {"HEADERS", (getter)Request_get_HEADERS, (setter)Request_set_HEADERS, "HEADERS", NULL}, */
  /* {"GET", (getter)Request_get_GET, (setter)Request_set_GET, "GET", NULL}, */
  /* {"POST", (getter)Request_get_POST, (setter)Request_set_POST, "POST", NULL}, */
  /* {"COOKIES", (getter)Request_get_COOKIES, (setter)Request_set_COOKIES, "COOKIES", NULL}, */
  /* {"SESSION", (getter)Request_get_SESSION, (setter)Request_set_SESSION, "SESSIONS", NULL}, */
  {NULL}
};

static PyMethodDef Request_methods[] = {
  {NULL}
};

static PyMemberDef Request_members[] = {
  {NULL}
};

PyTypeObject RequestType = {
  PyObject_HEAD_INIT(NULL)
  0,                            /* ob_size */
  "request.Request",            /* tp_name */
  sizeof(Request),              /* tp_basicsize */
  0,                            /* tp_itemsize */
  (destructor)Request_dealloc,  /* tp_dealloc */
  0,                            /* tp_print */
  0,                            /* tp_getattr */
  0,                            /* tp_setattr */
  0,                            /* tp_compare */
  0,                            /* tp_repr */
  0,                            /* tp_as_number */
  0,                            /* tp_as_sequence */
  0,                            /* tp_as_mapping */
  0,                            /* tp_hash */
  0,                            /* tp_call */
  0,                            /* tp_str */
  0,                            /* tp_getattro */
  0,                            /* tp_setattro */
  0,                            /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,           /* tp_flags */
  "Request objects",            /* tp_doc*/
  0,                            /* tp_traverse */
  0,                            /* tp_clear */
  0,                            /* tp_richcompare */
  0,                            /* tp_weaklistoffset */
  0,                            /* tp_iter */
  0,                            /* tp_iternext */
  Request_methods,              /* tp_methods */
  Request_members,              /* tp_members */
  Request_getseter,             /* tp_getset */
  0,                            /* tp_base */
  0,                            /* tp_dict */
  0,                            /* tp_descr_get */
  0,                            /* tp_descr_set */
  0,                            /* tp_dictoffset */
  (initproc)Request_init,       /* tp_init */
  0,                            /* tp_alloc */
  Request_new,                  /* tp_new */
};

static PyMethodDef request_module_methods[] = {
  {NULL}
};

PyObject *init_request(void) {
  PyObject* m;

  if (PyType_Ready(&RequestType) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.request", request_module_methods, "guava.request .");

  if (!m) {
    return NULL;
  }

  Py_INCREF(&RequestType);

  PyModule_AddObject(m, "Request", (PyObject *)&RequestType);

  return m;
}
