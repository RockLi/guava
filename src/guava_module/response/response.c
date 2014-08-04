/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_request.h"
#include "guava_module.h"
#include "guava_string.h"
#include "guava_memory.h"

static PyObject *Response_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  Response *self;

  self = (Response *)type->tp_alloc(type, 0);
  if (self) {
    self->res = guava_response_new();
  }

  return (PyObject *)self;
}

static void Response_dealloc(Response *self) {
  guava_response_free(self->res);

  self->ob_type->tp_free((PyObject *)self);
}

static int Response_init(Response *self, PyObject *args, PyObject *kwds) {
  return 0;
}

static PyObject *Response_get_major(Response *self, void *closure) {
  guava_response_t *res = self->res;

  return PyString_FromFormat("%d", res->major);
}

static PyObject *Response_get_minor(Response *self, void *closure) {
  guava_response_t *res = self->res;

  return PyString_FromFormat("%d", res->minor);
}

static PyObject *Response_get_status_code(Response *self, void *closure) {
  guava_response_t *res = self->res;

  return PyString_FromFormat("%d", res->status_code);
}

static PyGetSetDef Response_getseter[] = {
  {"major", (getter)Response_get_major, (setter)NULL, "major", NULL},
  {"minor", (getter)Response_get_minor, (setter)NULL, "minor", NULL},
  {"status_code", (getter)Response_get_status_code, (setter)NULL, "status_code", NULL},
  {NULL}
};

static PyMethodDef Response_methods[] = {
  {"set_header", (PyCFunction)Response_set_header, METH_VARARGS, "set the response header"},
  {"set_cookie", (PyCFunction)Response_set_cookie, METH_VARARGS, "set the response cookie"},
  {"set_status_code", (PyCFunction)Response_set_status_code, METH_VARARGS, "set the response status code"},
  {"write", (PyCFunction)Response_write, METH_VARARGS, "write data to the client"},
  {"redirect", (PyCFunction)Response_redirect, METH_VARARGS, "redirect to the other url"},
  {NULL}
};

static PyMemberDef Response_members[] = {
  {NULL}
};

PyTypeObject ResponseType = {
  PyObject_HEAD_INIT(NULL)
  0,                            /* ob_size */
  "response.Response",          /* tp_name */
  sizeof(Response),             /* tp_basicsize */
  0,                            /* tp_itemsize */
  (destructor)Response_dealloc, /* tp_dealloc */
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
  "Response objects",           /* tp_doc*/
  0,                            /* tp_traverse */
  0,                            /* tp_clear */
  0,                            /* tp_richcompare */
  0,                            /* tp_weaklistoffset */
  0,                            /* tp_iter */
  0,                            /* tp_iternext */
  Response_methods,             /* tp_methods */
  Response_members,             /* tp_members */
  Response_getseter,            /* tp_getset */
  0,                            /* tp_base */
  0,                            /* tp_dict */
  0,                            /* tp_descr_get */
  0,                            /* tp_descr_set */
  0,                            /* tp_dictoffset */
  (initproc)Response_init,      /* tp_init */
  0,                            /* tp_alloc */
  Response_new,                 /* tp_new */
};

static PyMethodDef response_module_methods[] = {
  {NULL}
};

PyObject *init_response(void) {
  PyObject* m;

  if (PyType_Ready(&ResponseType) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.response", response_module_methods, "guava.response .");

  if (!m) {
    return NULL;
  }

  Py_INCREF(&ResponseType);

  PyModule_AddObject(m, "Response", (PyObject *)&ResponseType);
  return m;
}
