/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_handler.h"
#include "guava_string.h"
#include "guava_module.h"
#include "guava_memory.h"

PyObject *StaticHandler_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  StaticHandler *self;

  self = (StaticHandler *)type->tp_alloc(type, 0);
  if (self) {
    self->handler = guava_handler_new();
  }

  return (PyObject *)self;
}

static void StaticHandler_dealloc(Handler *self) {
  if (self->handler) {
    guava_handler_free(self->handler);
    self->handler = NULL;
  }

  self->ob_type->tp_free((PyObject *)self);
}

static int StaticHandler_init(Handler *self, PyObject *args, PyObject *kwds) {
  char *url = NULL;
  char *local_path = NULL;

  if (!PyArg_ParseTuple(args,
                        "ss",
                        &url,
                        &local_path)) {
    return -1;
  }

  if (!url || !local_path) {
    PyErr_SetString(PyExc_TypeError, "url and local path not set correctly");
    return -1;
  }

  self->handler->flags |= GUAVA_HANDLER_VALID;
  self->handler->flags |= GUAVA_HANDLER_STATIC;

  self->handler->args = PyTuple_New(2);
  PyTuple_SetItem(self->handler->args, 0, PyString_FromString(url));
  PyTuple_SetItem(self->handler->args, 1, PyString_FromString(local_path));

  return 0;
}

static PyGetSetDef StaticHandler_getseter[] = {
  {NULL}
};

static PyMemberDef StaticHandler_members[] = {
  {NULL}
};

PyTypeObject StaticHandlerType = {
  PyObject_HEAD_INIT(NULL)
  0,                                    /* ob_size */
  "handler.StaticHandler",              /* tp_name */
  sizeof(StaticHandler),                /* tp_basicsize */
  0,                                    /* tp_itemsize */
  (destructor)StaticHandler_dealloc,    /* tp_dealloc */
  0,                                    /* tp_print */
  0,                                    /* tp_getattr */
  0,                                    /* tp_setattr */
  0,                                    /* tp_compare */
  0,                                    /* tp_repr */
  0,                                    /* tp_as_number */
  0,                                    /* tp_as_sequence */
  0,                                    /* tp_as_mapping */
  0,                                    /* tp_hash */
  0,                                    /* tp_call */
  0,                                    /* tp_str */
  0,                                    /* tp_getattro */
  0,                                    /* tp_setattro */
  0,                                    /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,                   /* tp_flags */
  "StaticHandler objects",              /* tp_doc*/
  0,                                    /* tp_traverse */
  0,                                    /* tp_clear */
  0,                                    /* tp_richcompare */
  0,                                    /* tp_weaklistoffset */
  0,                                    /* tp_iter */
  0,                                    /* tp_iternext */
  0,                                    /* tp_methods */
  StaticHandler_members,                /* tp_members */
  StaticHandler_getseter,               /* tp_getset */
  0,                                    /* tp_base */
  0,                                    /* tp_dict */
  0,                                    /* tp_descr_get */
  0,                                    /* tp_descr_set */
  0,                                    /* tp_dictoffset */
  (initproc)StaticHandler_init,         /* tp_init */
  0,                                    /* tp_alloc */
  StaticHandler_new,                    /* tp_new */
};
