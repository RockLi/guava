/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_module.h"
#include "guava_module_session.h"
#include "guava_session/guava_session.h"
#include "guava_memory.h"

static PyObject *SessionStore_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  SessionStore *self;

  self = (SessionStore *)type->tp_alloc(type, 0);
  if (self) {
    self->store = NULL;
  }

  return (PyObject *)self;
}

static void SessionStore_dealloc(SessionStore *self) {
  if (self->store) {
    guava_session_store_free((guava_session_store_t *)self->store);
  }

  ((SessionStore *)self)->ob_type->tp_free((PyObject *)self);
}

static int SessionStore_init(SessionStore *self, PyObject *args, PyObject *kwds) {
  char *kwlist[] = {"type", NULL};
  int type = GUAVA_SESSION_STORE_CUSTOM;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "|i",
                                   kwlist,
                                   &type)) {
    return -1;
  }

  if (type != GUAVA_SESSION_STORE_CUSTOM) {
    /* User created a new class inherited from the base class */
    self->store = guava_session_store_new(type);
  }

  return 0;
}

static PyObject *SessionStore_set(SessionStore *self, PyObject *args) {
  char *id = NULL;
  PyObject *items = NULL;
  if (!PyArg_ParseTuple(args,
                        "sO",
                        &id,
                        &items)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  if (!PyDict_Check(items)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  Py_INCREF(items);
  guava_session_set(self->store, (guava_session_id_t)id, items);

  Py_RETURN_NONE;
}

static PyObject *SessionStore_get(SessionStore *self, PyObject *args) {
  char *id = NULL;
  if (!PyArg_ParseTuple(args,
                        "s",
                        &id)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  PyObject *v = guava_session_get(self->store, (guava_session_id_t)id);

  Py_INCREF(v);
  return v;
}

static PyObject *SessionStore_delete(SessionStore *self, PyObject *args) {
  char *id = NULL;
  if (!PyArg_ParseTuple(args,
                        "s",
                        &id)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  PyObject *v = guava_session_delete(self->store, (guava_session_id_t)id);
  Py_INCREF(v);

  return v;
}

static PyObject *SessionStore_clear(SessionStore *self, PyObject *args) {
  char *id = NULL;
  if (!PyArg_ParseTuple(args,
                        "s",
                        &id)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  PyObject *v = PyDict_New();
  Py_INCREF(v);

  guava_session_set(self->store, (guava_session_id_t)id, v);

  Py_RETURN_NONE;
}

static PyObject *session_generate_id(PyObject *self, PyObject *args) {
  return PyString_FromString(guava_session_new_id());
}

static PyGetSetDef SessionStore_getseter[] = {
  {NULL}
};

static PyMemberDef SessionStore_members[] = {
  {NULL}
};

static PyMethodDef SessionStore_methods[] = {
  {"set", (PyCFunction)SessionStore_set, METH_VARARGS, "set"},
  {"get", (PyCFunction)SessionStore_get, METH_VARARGS, "get"},
  {"delete", (PyCFunction)SessionStore_delete, METH_VARARGS, "delete"},
  {"clear", (PyCFunction)SessionStore_clear, METH_VARARGS, "clear"},
  {NULL}
};

PyTypeObject SessionStoreType = {
  PyObject_HEAD_INIT(NULL)
  0,                                            /* ob_size */
  "session.SessionStore",                       /* tp_name */
  sizeof(SessionStore),                         /* tp_basicsize */
  0,                                             /* tp_itemsize */
  (destructor)SessionStore_dealloc,              /* tp_dealloc */
  0,                                        /* tp_print */
  0,                                        /* tp_getattr */
  0,                                        /* tp_setattr */
  0,                                        /* tp_compare */
  0,                                        /* tp_repr */
  0,                                        /* tp_as_number */
  0,                                        /* tp_as_sequence */
  0,                                        /* tp_as_mapping */
  0,                                        /* tp_hash */
  0,                                        /* tp_call */
  0,                                        /* tp_str */
  0,                                        /* tp_getattro */
  0,                                        /* tp_setattro */
  0,                                        /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
  "SessionStore objects",                        /* tp_doc*/
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  0,                                        /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  SessionStore_methods,                          /* tp_methods */
  SessionStore_members,                          /* tp_members */
  SessionStore_getseter,                         /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  (initproc)SessionStore_init,                   /* tp_init */
  0,                                        /* tp_alloc */
  SessionStore_new,                              /* tp_new */
};

static PyMethodDef session_module_methods[] = {
  {"generate_id", session_generate_id, METH_NOARGS, "generate a new sid"},
  {NULL}
};

PyObject *init_session(void) {
  PyObject* m;

  if (PyType_Ready(&SessionStoreType) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.session", session_module_methods, "guava.session .");
  if (!m) {
    return NULL;
  }

  Py_INCREF(&SessionStoreType);

  PyModule_AddObject(m, "SessionStore", (PyObject *)&SessionStoreType);

  PyModule_AddIntConstant(m, "Mem", GUAVA_SESSION_STORE_INMEM);
  PyModule_AddIntConstant(m, "File", GUAVA_SESSION_STORE_FILE);

  return m;
}
