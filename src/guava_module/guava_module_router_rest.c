/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_router/guava_router.h"
#include "guava_module.h"
#include "guava_module_router.h"
#include "guava_session/guava_session.h"

static PyObject *RESTRouter_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  RESTRouter *self;

  self = (RESTRouter *)type->tp_alloc(type, 0);
  if (self) {
    self->router.router = (guava_router_t *)guava_router_rest_new();
  }

  return (PyObject *)self;
}

static void RESTRouter_dealloc(RESTRouter *self) {
  guava_router_rest_free((guava_router_rest_t *)self->router.router);

  ((Router *)self)->ob_type->tp_free((PyObject *)self);
}

static int RESTRouter_init(RESTRouter *self, PyObject *args, PyObject *kwds) {
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

static PyObject *RESTRouter_repr(RESTRouter *self) {
  guava_router_rest_t *router = (guava_router_rest_t *)self->router.router;
  return PyString_FromFormat("RESTRouter path(%s)",
                             router->route.mount_point);
}

static PyObject *RESTRouter_get_mount_point(RESTRouter *self, void *closure) {
  guava_router_rest_t *router = (guava_router_rest_t *)self->router.router;
  return PyString_FromString(router->route.mount_point);
}

static int RESTRouter_set_mount_point(RESTRouter *self, PyObject *value, void *closure) {
  guava_router_rest_t *router = (guava_router_rest_t *)self->router.router;
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

static PyObject *RESTRouter_route(RESTRouter *self, PyObject *args) {
  PyObject *req;

  if (!PyArg_ParseTuple(args, "O", &req)) {
    PyErr_SetString(PyExc_TypeError, "request object needed");
    return NULL;
  }

  guava_handler_t *handler = guava_handler_new();
  guava_router_rest_route((guava_router_rest_t *)self->router.router, ((Request *)req)->req, handler);

  if (!guava_handler_is_valid(handler)) {
    guava_handler_free(handler);
    Py_RETURN_NONE;
  }

  Handler *handler_obj = (Handler *)Handler_new(&HandlerType, NULL, NULL);

  if (handler_obj->handler) {
    guava_handler_free(handler_obj->handler);
    handler_obj->handler = NULL;
  }

  handler_obj->handler = handler;

  return (PyObject *)handler_obj;
}

static PyGetSetDef RESTRouter_getseter[] = {
  {"mount_point", (getter)RESTRouter_get_mount_point, (setter)RESTRouter_set_mount_point, "the mount point of the router", NULL},
  {NULL}
};

static PyMemberDef RESTRouter_members[] = {
  {NULL}
};

static PyMethodDef RESTRouter_methods[] = {
  {"route", (PyCFunction)RESTRouter_route, METH_VARARGS, "route"},
  {NULL}
};

PyTypeObject RESTRouterType = {
  PyObject_HEAD_INIT(NULL)
  0,                              /* ob_size */
  "router.RESTRouter",            /* tp_name */
  sizeof(RESTRouter),             /* tp_basicsize */
  0,                              /* tp_itemsize */
  (destructor)RESTRouter_dealloc, /* tp_dealloc */
  0,                              /* tp_print */
  0,                              /* tp_getattr */
  0,                              /* tp_setattr */
  0,                              /* tp_compare */
  (reprfunc)RESTRouter_repr,      /* tp_repr */
  0,                              /* tp_as_number */
  0,                              /* tp_as_sequence */
  0,                              /* tp_as_mapping */
  0,                              /* tp_hash */
  0,                              /* tp_call */
  0,                              /* tp_str */
  0,                              /* tp_getattro */
  0,                              /* tp_setattro */
  0,                              /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,             /* tp_flags */
  "RESTRouter objects",           /* tp_doc*/
  0,                              /* tp_traverse */
  0,                              /* tp_clear */
  0,                              /* tp_richcompare */
  0,                              /* tp_weaklistoffset */
  0,                              /* tp_iter */
  0,                              /* tp_iternext */
  RESTRouter_methods,             /* tp_methods */
  RESTRouter_members,             /* tp_members */
  RESTRouter_getseter,            /* tp_getset */
  &RouterType,                    /* tp_base */
  0,                              /* tp_dict */
  0,                              /* tp_descr_get */
  0,                              /* tp_descr_set */
  0,                              /* tp_dictoffset */
  (initproc)RESTRouter_init,      /* tp_init */
  0,                              /* tp_alloc */
  RESTRouter_new,                 /* tp_new */
};
