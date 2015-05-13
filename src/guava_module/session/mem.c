/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_memory.h"
#include "guava_url.h"
#include "guava_session/guava_session_handler.h"
#include "guava_module/guava_module.h"
#include "guava_module/guava_module_session_store.h"

static PyObject *
Py_MemSessionStore_read(Py_MemSessionStore *self,
                        PyObject           *args)
{
  const char *sid = NULL;
  if (!PyArg_ParseTuple(args, "s", &sid)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  PyObject *v = PyDict_GetItemString(self->data, sid);
  if (!v) {
    Py_RETURN_NONE;
  }

  Py_INCREF(v);
  return v;
}

static PyObject *
Py_MemSessionStore_write(Py_MemSessionStore *self,
                         PyObject           *args)
{
  const char *sid = NULL;
  PyObject     *v = NULL;
  if (!PyArg_ParseTuple(args, "sO", &sid, &v)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  PyDict_SetItemString(self->data, sid, v);
  Py_RETURN_NONE;
}

static PyObject *
Py_MemSessionStore_destroy(Py_MemSessionStore *self,
                           PyObject           *args)
{
  const char *sid = NULL;
  if (!PyArg_ParseTuple(args, "s", &sid)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  PyDict_DelItemString(self->data, sid);
  Py_RETURN_NONE;
}

static PyMethodDef Py_MemSessionStore_methods[] = {
    {"read", (PyCFunction)Py_MemSessionStore_read, METH_VARARGS, PyDoc_STR("read the data")},
    {"write", (PyCFunction)Py_MemSessionStore_write, METH_VARARGS, PyDoc_STR("write the data")},
    {"destroy", (PyCFunction)Py_MemSessionStore_destroy, METH_VARARGS, PyDoc_STR("destroy the data")},
    {NULL,	NULL},
};

static int
Py_MemSessionStore_init(Py_MemSessionStore *self,
                        PyObject           *args,
                        PyObject           *kwds)
{
  if (PyType_SessionStore.tp_init((PyObject *)self, args, kwds) < 0) {
        return -1;
  }

  self->data = PyDict_New();
  return 0;
}

PyTypeObject PyType_MemSessionStore = {
    PyObject_HEAD_INIT(NULL)
    0,                                      /* ob_size */
    "session.MemSessionStore",              /* tp_name */
    sizeof(Py_MemSessionStore),             /* tp_basicsize */
    0,                                      /* tp_itemsize */
    0,                                      /* tp_dealloc */
    0,                                      /* tp_print */
    0,                                      /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_compare */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    0,                                      /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, /* tp_flags */
    0,                                      /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    Py_MemSessionStore_methods,             /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)Py_MemSessionStore_init,      /* tp_init */
    0,                                      /* tp_alloc */
    0,                                      /* tp_new */
};
