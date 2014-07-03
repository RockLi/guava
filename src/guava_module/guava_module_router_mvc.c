/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_router/guava_router.h"
#include "guava_handler.h"
#include "guava_module.h"
#include "guava_module_router.h"
#include "guava_session/guava_session.h"
#include "guava_memory.h"

static PyObject *MVCRouter_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  MVCRouter *self;

  self = (MVCRouter *)type->tp_alloc(type, 0);
  if (self) {
    self->router.router = (guava_router_t *)guava_router_mvc_new();
  }

  return (PyObject *)self;
}

static void MVCRouter_dealloc(MVCRouter *self) {
  guava_router_mvc_free((guava_router_mvc_t *)self->router.router);

  ((Router *)self)->ob_type->tp_free((PyObject *)self);
}

static int MVCRouter_init(MVCRouter *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"mount_point", "session_store", NULL};

  char *mount_point = NULL;
  SessionStore *session_store = NULL;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "s|O",
                                   kwlist,
                                   &mount_point,
                                   &session_store)) {
    return -1;
  }

  guava_router_set_mount_point((guava_router_t *)self->router.router, mount_point);

  if (session_store) {
    Py_INCREF(session_store);
    guava_router_set_session_store((guava_router_t *)self->router.router, session_store->store);
  }

  return 0;
}

static PyObject *MVCRouter_repr(MVCRouter *self) {
  guava_router_mvc_t *router = (guava_router_mvc_t *)self->router.router;
  return PyString_FromFormat("MVCRouter path(%s)",
                             router->route.mount_point);
}

static PyObject *MVCRouter_get_mount_point(MVCRouter *self, void *closure) {
  guava_router_mvc_t *router = (guava_router_mvc_t *)self->router.router;
  return PyString_FromString(router->route.mount_point);
}

static int MVCRouter_set_mount_point(MVCRouter *self, PyObject *value, void *closure) {
  guava_router_mvc_t *router = (guava_router_mvc_t *)self->router.router;
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

static PyObject *MVCRouter_route(MVCRouter *self, PyObject *args) {
  PyObject *req;

  if (!PyArg_ParseTuple(args, "O", &req)) {
    PyErr_SetString(PyExc_TypeError, "request object needed");
    return NULL;
  }

  guava_handler_t *handler = guava_handler_new();
  guava_router_mvc_route((guava_router_mvc_t *)self->router.router, ((Request *)req)->req, handler);

  if (!guava_handler_is_valid(handler)) {
    guava_handler_free(handler);
    Py_RETURN_NONE;
  }

  Handler *handler_obj = PyObject_New(Handler, &HandlerType);
  handler_obj->handler = handler;

  return (PyObject *)handler_obj;
}

static PyGetSetDef MVCRouter_getseter[] = {
  {"mount_point", (getter)MVCRouter_get_mount_point, (setter)MVCRouter_set_mount_point, "path", NULL},
  {NULL}
};

static PyMemberDef MVCRouter_members[] = {
  {NULL}
};

static PyMethodDef MVCRouter_methods[] = {
  {"route", (PyCFunction)MVCRouter_route, METH_VARARGS, "route"},
  {NULL}
};

PyTypeObject MVCRouterType = {
  PyObject_HEAD_INIT(NULL)
  0,                             /* ob_size */
  "router.MVCRouter",            /* tp_name */
  sizeof(MVCRouter),             /* tp_basicsize */
  0,                             /* tp_itemsize */
  (destructor)MVCRouter_dealloc, /* tp_dealloc */
  0,                             /* tp_print */
  0,                             /* tp_getattr */
  0,                             /* tp_setattr */
  0,                             /* tp_compare */
  (reprfunc)MVCRouter_repr,      /* tp_repr */
  0,                             /* tp_as_number */
  0,                             /* tp_as_sequence */
  0,                             /* tp_as_mapping */
  0,                             /* tp_hash */
  0,                             /* tp_call */
  0,                             /* tp_str */
  0,                             /* tp_getattro */
  0,                             /* tp_setattro */
  0,                             /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,            /* tp_flags */
  "MVCRouter objects",           /* tp_doc*/
  0,                             /* tp_traverse */
  0,                             /* tp_clear */
  0,                             /* tp_richcompare */
  0,                             /* tp_weaklistoffset */
  0,                             /* tp_iter */
  0,                             /* tp_iternext */
  MVCRouter_methods,             /* tp_methods */
  MVCRouter_members,             /* tp_members */
  MVCRouter_getseter,            /* tp_getset */
  &RouterType,                   /* tp_base */
  0,                             /* tp_dict */
  0,                             /* tp_descr_get */
  0,                             /* tp_descr_set */
  0,                             /* tp_dictoffset */
  (initproc)MVCRouter_init,      /* tp_init */
  0,                             /* tp_alloc */
  MVCRouter_new,                 /* tp_new */
};
