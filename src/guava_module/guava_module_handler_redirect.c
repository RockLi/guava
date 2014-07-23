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

PyObject *RedirectHandler_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  RedirectHandler *self;

  self = (RedirectHandler *)type->tp_alloc(type, 0);
  if (self) {
    self->handler = guava_handler_new();
  }

  return (PyObject *)self;
}

static void RedirectHandler_dealloc(Handler *self) {
  if (self->handler) {
    guava_handler_free(self->handler);
    self->handler = NULL;
  }

  self->ob_type->tp_free((PyObject *)self);
}

static int RedirectHandler_init(Handler *self, PyObject *args, PyObject *kwds) {
  char *url;

  if (!PyArg_ParseTuple(args,
                        "s",
                        &url)) {
    return -1;
  }

  self->handler->flags |= GUAVA_HANDLER_REDIRECT;
  self->handler->args = PyTuple_New(1);
  PyTuple_SetItem(self->handler->args, 0, PyString_FromString(url));

  guava_handler_mark_valid(self->handler);
  return 0;
}

static PyGetSetDef RedirectHandler_getseter[] = {
  {NULL}
};

static PyMemberDef RedirectHandler_members[] = {
  {NULL}
};

PyTypeObject RedirectHandlerType = {
  PyObject_HEAD_INIT(NULL)
  0,                                    /* ob_size */
  "handler.RedirectHandler",            /* tp_name */
  sizeof(RedirectHandler),              /* tp_basicsize */
  0,                                    /* tp_itemsize */
  (destructor)RedirectHandler_dealloc,  /* tp_dealloc */
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
  "RedirectHandler objects",            /* tp_doc*/
  0,                                    /* tp_traverse */
  0,                                    /* tp_clear */
  0,                                    /* tp_richcompare */
  0,                                    /* tp_weaklistoffset */
  0,                                    /* tp_iter */
  0,                                    /* tp_iternext */
  0,                                    /* tp_methods */
  RedirectHandler_members,              /* tp_members */
  RedirectHandler_getseter,             /* tp_getset */
  0,                                    /* tp_base */
  0,                                    /* tp_dict */
  0,                                    /* tp_descr_get */
  0,                                    /* tp_descr_set */
  0,                                    /* tp_dictoffset */
  (initproc)RedirectHandler_init,       /* tp_init */
  0,                                    /* tp_alloc */
  RedirectHandler_new,                  /* tp_new */
};
