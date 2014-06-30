/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_router/guava_router.h"
#include "guava_module.h"
#include "guava_module_router.h"

static PyObject *StaticRouter_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  StaticRouter *self;

  self = (StaticRouter *)type->tp_alloc(type, 0);
  if (self) {
    self->router.router = (guava_router_t *)guava_router_static_new();
  }

  return (PyObject *)self;
}

static void StaticRouter_dealloc(StaticRouter *self) {
  guava_router_static_free((guava_router_static_t *)self->router.router);

  ((Router *)self)->ob_type->tp_free((PyObject *)self);
}

static int StaticRouter_init(StaticRouter *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"mount_point", "directory", "allow_index", "session_store", NULL};

  char *mount_point = NULL;
  char *directory = NULL;
  char allow_index = 0;
  SessionStore *session_store = NULL;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "ss|bO",
                                   kwlist,
                                   &mount_point,
                                   &directory,
                                   &allow_index,
                                   &session_store)) {
    return -1;
  }

  guava_router_set_mount_point((guava_router_t *)self->router.router, mount_point);
  guava_router_static_set_directory((guava_router_static_t *)self->router.router, directory);
  guava_router_static_set_allow_index((guava_router_static_t *)self->router.router, allow_index);

  Py_INCREF(session_store);
  guava_router_set_session_store((guava_router_t *)self->router.router, session_store->store);

  return 0;
}

static PyObject *StaticRouter_repr(StaticRouter *self) {
  guava_router_static_t *router = (guava_router_static_t *)self->router.router;
  return PyString_FromFormat("StaticRouter path(%s), directory(%s), allow_index(%s)",
                             router->route.mount_point,
                             router->directory,
                             router->allow_index ? "TRUE" : "FALSE");
}

static PyObject *StaticRouter_get_mount_point(StaticRouter *self, void *closure) {
  guava_router_static_t *router = (guava_router_static_t *)self->router.router;
  return PyString_FromString(router->route.mount_point);
}

static int StaticRouter_set_mount_point(StaticRouter *self, PyObject *value, void *closure) {
  guava_router_static_t *router = (guava_router_static_t *)self->router.router;
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the path attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The path attribute value must be a string");
    return -1;
  }

  guava_router_set_mount_point((guava_router_t *)router, PyString_AsString(value));
  return 0;
}

static PyObject *StaticRouter_get_directory(StaticRouter *self, void *closure) {
  guava_router_static_t *router = (guava_router_static_t *)self->router.router;
  return PyString_FromString(router->directory);
}

static int StaticRouter_set_directory(StaticRouter *self, PyObject *value, void *closure) {
  guava_router_static_t *router = (guava_router_static_t *)self->router.router;
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the directory attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The directory attribute value must be a string");
    return -1;
  }

  guava_router_static_set_directory(router, PyString_AsString(value));
  return 0;
}

static PyObject *StaticRouter_get_allow_index(StaticRouter *self, void *closure) {
  guava_router_static_t *router = (guava_router_static_t *)self->router.router;
  return PyBool_FromLong(router->allow_index);
}

static int StaticRouter_set_allow_index(StaticRouter *self, PyObject *value, void *closure) {
  guava_router_static_t *router = (guava_router_static_t *)self->router.router;
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the allow_index attribute");
    return -1;
  }

  if (!PyBool_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The directory allow_index value must be a boolean");
    return -1;
  }

  guava_router_static_set_allow_index(router, (guava_bool_t )PyInt_AsLong(value));
  return 0;
}

static PyObject *StaticRouter_route(StaticRouter *self, PyObject *args) {
  Py_RETURN_NONE;
}

static PyGetSetDef StaticRouter_getseter[] = {
  {"mount_point", (getter)StaticRouter_get_mount_point, (setter)StaticRouter_set_mount_point, "the mount point of the router", NULL},
  {"directory", (getter)StaticRouter_get_directory, (setter)StaticRouter_set_directory, "directory", NULL},
  {"allow_index", (getter)StaticRouter_get_allow_index, (setter)StaticRouter_set_allow_index, "allow index", NULL},
  {NULL}
};

static PyMemberDef StaticRouter_members[] = {
  {NULL}
};

static PyMethodDef StaticRouter_methods[] = {
  {"route", (PyCFunction)StaticRouter_route, METH_VARARGS, "route"},
  {NULL}
};

PyTypeObject StaticRouterType = {
  PyObject_HEAD_INIT(NULL)
  0,                                 /* ob_size */
  "router.StaticRouter",             /* tp_name */
  sizeof(StaticRouter),              /* tp_basicsize */
  0,                                 /* tp_itemsize */
  (destructor)StaticRouter_dealloc,  /* tp_dealloc */
  0,                                 /* tp_print */
  0,                                 /* tp_getattr */
  0,                                 /* tp_setattr */
  0,                                 /* tp_compare */
  (reprfunc)StaticRouter_repr,       /* tp_repr */
  0,                                 /* tp_as_number */
  0,                                 /* tp_as_sequence */
  0,                                 /* tp_as_mapping */
  0,                                 /* tp_hash */
  0,                                 /* tp_call */
  0,                                 /* tp_str */
  0,                                 /* tp_getattro */
  0,                                 /* tp_setattro */
  0,                                 /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,                /* tp_flags */
  "StaticRouter objects",            /* tp_doc*/
  0,                                 /* tp_traverse */
  0,                                 /* tp_clear */
  0,                                 /* tp_richcompare */
  0,                                 /* tp_weaklistoffset */
  0,                                 /* tp_iter */
  0,                                 /* tp_iternext */
  StaticRouter_methods,              /* tp_methods */
  StaticRouter_members,              /* tp_members */
  StaticRouter_getseter,             /* tp_getset */
  &RouterType,                       /* tp_base */
  0,                                 /* tp_dict */
  0,                                 /* tp_descr_get */
  0,                                 /* tp_descr_set */
  0,                                 /* tp_dictoffset */
  (initproc)StaticRouter_init,       /* tp_init */
  0,                                 /* tp_alloc */
  StaticRouter_new,                  /* tp_new */
};
