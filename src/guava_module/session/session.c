/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_module/guava_module.h"
#include "guava_memory.h"
#include "guava_url.h"
#include "guava_module/guava_module_session_store.h"
#include "guava_session/guava_session_handler.h"

static PyObject *
Py_SessionStore_new(PyTypeObject *type,
                    PyObject     *args,
                    PyObject     *kwds)
{
  Py_SessionStore *self = NULL;

  self = (Py_SessionStore *)type->tp_alloc(type, 0);
  if (self) self->store = NULL;

  return (PyObject *)self;
}

static void
Py_SessionStore_dealloc(Py_SessionStore *self)
{
  if (self->store) {
    guava_session_store_destroy(self->store);
  }
  self->ob_type->tp_free((PyObject *)self);
}

static int
Py_SessionStore_init(Py_SessionStore *self,
                     PyObject        *args,
                     PyObject        *kwds)
{
  static char *kwlist[] = {"name", "gc_time", "cookie_expired", "cookie_path", "cookie_domain", "cookie_secure", "cookie_httponly", NULL};

  char            *name = NULL;
  long          gc_time = 0;
  long   cookie_expired = 0;
  char     *cookie_path = NULL;
  char   *cookie_domain = NULL;
  int     cookie_secure = GUAVA_FALSE;
  int   cookie_httponly = GUAVA_FALSE;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "|siissBB",
                                   kwlist,
                                   &name,
                                   &gc_time,
                                   &cookie_expired,
                                   &cookie_path,
                                   &cookie_domain,
                                   &cookie_secure,
                                   &cookie_httponly)) {
    return -1;
  }

  self->store = guava_session_store_new(name ? name : "sid",
                                        (int)gc_time,
                                        (int)cookie_expired,
                                        cookie_path,
                                        cookie_domain,
                                        cookie_secure,
                                        cookie_httponly,
                                        guava_session_handler_create_session_id_default,
                                        NULL,
                                        NULL,
                                        NULL);

  if (!self->store) {
    PyErr_SetString(PyExc_TypeError, "failed to initialize a new session store object");
    return -1;
  }

  return 0;
}

static PyObject *
Py_SessionStore_repr(Py_SessionStore *self) {
  return PyString_FromFormat("%s", "SessionStore");
}

static PyObject *
Py_SessionStore_get_name(Py_SessionStore *self,
                         void            *closure)
{
  if (self->store->name) {
    return PyString_FromString(self->store->name);
  }

  Py_RETURN_NONE;
}

static int
Py_SessionStore_set_name(Py_SessionStore *self,
                         PyObject        *value,
                         void            *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the name property");

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The name attribute value must be a string");
    return -1;
  }

  if (!guava_session_store_set_name(self->store,
                                    PyString_AsString(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the name attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_SessionStore_get_gc_time(Py_SessionStore *self,
                            void            *closure)
{
  return PyInt_FromLong(self->store->gc_time);
}

static int
Py_SessionStore_set_gc_time(Py_SessionStore *self,
                            PyObject        *value,
                            void            *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the gc_time property");

  if (!PyInt_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The gc_time attribute value must be a int");
    return -1;
  }

  if (!guava_session_store_set_gc_time(self->store,
                                       (int)PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the gc_time attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_SessionStore_get_cookie_expired(Py_SessionStore *self,
                                   void            *closure)
{
  return PyInt_FromLong(self->store->cookie_expired);
}

static int
Py_SessionStore_set_cookie_expired(Py_SessionStore *self,
                                   PyObject        *value,
                                   void            *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the cookie_expired property");

  if (!PyInt_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The cookie_expired attribute value must be a int");
    return -1;
  }

  if (!guava_session_store_set_cookie_expired(self->store,
                                              (int)PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the cookie_expired attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_SessionStore_get_cookie_path(Py_SessionStore *self,
                                void            *closure)
{
  if (self->store->cookie_path) {
    return PyString_FromString(self->store->cookie_path);
  }

  Py_RETURN_NONE;
}

static int
Py_SessionStore_set_cookie_path(Py_SessionStore *self,
                                PyObject        *value,
                                void            *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the cookie_path property");

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The name attribute cookie_path must be a string");
    return -1;
  }

  if (!guava_session_store_set_cookie_path(self->store,
                                           PyString_AsString(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the cookie_path attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_SessionStore_get_cookie_domain(Py_SessionStore *self,
                                  void            *closure)
{
  if (self->store->cookie_domain) {
    return PyString_FromString(self->store->cookie_domain);
  }

  Py_RETURN_NONE;
}

static int
Py_SessionStore_set_cookie_domain(Py_SessionStore *self,
                                  PyObject        *value,
                                  void            *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the cookie_domain property");

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The name attribute cookie_domain must be a string");
    return -1;
  }

  if (!guava_session_store_set_cookie_domain(self->store,
                                           PyString_AsString(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the cookie_domain attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_SessionStore_get_cookie_secure(Py_SessionStore *self,
                                      void            *closure)
{
  return PyBool_FromLong(self->store->cookie_secure);
}

static int
Py_SessionStore_set_cookie_secure(Py_SessionStore *self,
                                  PyObject        *value,
                                  void            *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the cookie_secure property");

  if (!PyBool_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The cookie_secure attribute value must be a boolean");
    return -1;
  }

  if (!guava_session_store_set_cookie_secure(self->store, (int)PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the cookie_secure attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_SessionStore_get_cookie_httponly(Py_SessionStore *self,
                                    void            *closure)
{
  return PyBool_FromLong(self->store->cookie_httponly);
}

static int
Py_SessionStore_set_cookie_httponly(Py_SessionStore *self,
                                    PyObject        *value,
                                    void            *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the cookie_httponly property");

  if (!PyBool_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The cookie_httponly attribute value must be a boolean");
    return -1;
  }

  if (!guava_session_store_set_cookie_httponly(self->store,
                                               (int)PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the cookie_httponly attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_SessionStore_new_session_id(Py_SessionStore *self,
                               PyObject        *unused)
{
  if (!self->store->create_session_id_handler) {
    Py_RETURN_NONE;
  }

  guava_string_t sid = self->store->create_session_id_handler();
  return PyString_FromString(sid);
}

static PyMethodDef Py_SessionStore_methods[] = {
  {"new_session_id", (PyCFunction)Py_SessionStore_new_session_id, METH_NOARGS, "get a new session id"},
  {NULL}
};

static PyGetSetDef Py_SessionStore_getseter[] = {
  {"name", (getter)Py_SessionStore_get_name, (setter)Py_SessionStore_set_name, "name", NULL},
  {"gc_time", (getter)Py_SessionStore_get_gc_time, (setter)Py_SessionStore_set_gc_time, "gc_time", NULL},
  {"cookie_expired", (getter)Py_SessionStore_get_cookie_expired, (setter)Py_SessionStore_set_cookie_expired, "cookie expired", NULL},
  {"cookie_path", (getter)Py_SessionStore_get_cookie_path, (setter)Py_SessionStore_set_cookie_path, "cookie path", NULL},
  {"cookie_domain", (getter)Py_SessionStore_get_cookie_domain, (setter)Py_SessionStore_set_cookie_domain, "cookie domain", NULL},
  {"cookie_secure", (getter)Py_SessionStore_get_cookie_secure, (setter)Py_SessionStore_set_cookie_secure, "cookie secure", NULL},
  {"cookie_httponly", (getter)Py_SessionStore_get_cookie_httponly, (setter)Py_SessionStore_set_cookie_httponly, "cookie httponly", NULL},
  {NULL}
};

static PyMemberDef Py_SessionStore_members[] = {
  {NULL}
};

PyTypeObject PyType_SessionStore = {
  PyObject_HEAD_INIT(NULL)
  0,                                      /* ob_size */
  "session.SessionStore",                 /* tp_name */
  sizeof(Py_SessionStore),                /* tp_basicsize */
  0,                                      /* tp_itemsize */
  (destructor)Py_SessionStore_dealloc,    /* tp_dealloc */
  0,                                      /* tp_print */
  0,                                      /* tp_getattr */
  0,                                      /* tp_setattr */
  0,                                      /* tp_compare */
  (reprfunc)Py_SessionStore_repr,         /* tp_repr */
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
  "SessionStore objects",                 /* tp_doc*/
  0,                                      /* tp_traverse */
  0,                                      /* tp_clear */
  0,                                      /* tp_richcompare */
  0,                                      /* tp_weaklistoffset */
  0,                                      /* tp_iter */
  0,                                      /* tp_iternext */
  Py_SessionStore_methods,                /* tp_methods */
  Py_SessionStore_members,                /* tp_members */
  Py_SessionStore_getseter,               /* tp_getset */
  0,                                      /* tp_base */
  0,                                      /* tp_dict */
  0,                                      /* tp_descr_get */
  0,                                      /* tp_descr_set */
  0,                                      /* tp_dictoffset */
  (initproc)Py_SessionStore_init,         /* tp_init */
  0,                                      /* tp_alloc */
  Py_SessionStore_new,                    /* tp_new */
};

static PyMethodDef Py_session_module_methods[] = {
  {NULL}
};

PyObject *
init_session_module(void)
{
  PyObject *m = NULL;
  m = Py_InitModule3("guava.session", Py_session_module_methods, "guava.session");
  if (!m) return NULL;

  if (PyType_Ready(&PyType_SessionStore) < 0) return NULL;
  Py_INCREF(&PyType_SessionStore);
  PyModule_AddObject(m, "SessionStore", (PyObject *)&PyType_SessionStore);

  PyType_MemSessionStore.tp_base = &PyType_SessionStore;
  if (PyType_Ready(&PyType_MemSessionStore) < 0) return NULL;
  Py_INCREF(&PyType_MemSessionStore);
  PyModule_AddObject(m, "MemSessionStore", (PyObject *)&PyType_MemSessionStore);

  PyType_FileSessionStore.tp_base = &PyType_SessionStore;
  if (PyType_Ready(&PyType_FileSessionStore) < 0) return NULL;
  Py_INCREF(&PyType_FileSessionStore);
  PyModule_AddObject(m, "FileSessionStore", (PyObject *)&PyType_FileSessionStore);

  return m;
}
