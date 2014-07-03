/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_module.h"
#include "guava_module_router.h"
#include "guava_memory.h"

static PyObject *Router_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  Router *self;

  self = (Router *)type->tp_alloc(type, 0);
  if (self) {
    self->router = guava_router_new();
  }

  return (PyObject *)self;
}

static void Router_dealloc(Router *self) {
  ((Router *)self)->ob_type->tp_free((PyObject *)self);
}

static int Router_init(Router *self, PyObject *args, PyObject *kwds) {
  char *kwlist[] = {"routes", NULL};

  PyObject *routes = NULL;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "|O",
                                   kwlist,
                                   &routes)) {
    return -1;
  }

  if (routes && !PyDict_Check(routes)) {
    return -1;
  }

  if (routes) {
    Py_INCREF(routes);
    guava_router_set_routes(self->router, routes);
  }

  return 0;
}

static PyObject *Router_route(Router *self, PyObject *args) {
  if (!self->router->routes) {
    Py_RETURN_NONE;
  }

  PyObject *req = NULL;

  if (!PyArg_ParseTuple(args,
                        "O",
                        &req)) {
    PyErr_SetString(PyExc_TypeError, "error parameter");
    return NULL;
  }

  if (!PyObject_TypeCheck(req, &RequestType)) {
    PyErr_SetString(PyExc_TypeError, "the first parameter must be the request object");
    return NULL;
  }

  PyObject *key = NULL, *value = NULL;
  Py_ssize_t pos = 0;

  while (PyDict_Next(self->router->routes, &pos, &key, &value)) {
    if (strcmp(((Request *)req)->req->url, PyString_AsString(key)) == 0) {
      Py_INCREF(value);
      return value;
    }
  }

  Py_RETURN_NONE;
}

static PyObject *Router_routes(Router *self, PyObject *args) {
  if (!self->router->routes){
    self->router->routes = PyDict_New();
  }

  Py_INCREF(self->router->routes);

  return self->router->routes;
}

static PyObject *Router_register(Router *self, PyObject *args) {
  char *path = NULL;
  PyObject *o = NULL;

  if (!PyArg_ParseTuple(args,
                        "sO",
                        &path,
                        &o)) {
    PyErr_SetString(PyExc_TypeError, "not acceptable parameters");
    return NULL;
  }

  assert(path != NULL);
  assert(o != NULL);

  Py_INCREF(o);
  PyDict_SetItemString(self->router->routes, path, o);

  Py_RETURN_TRUE;
}

static PyGetSetDef Router_getseter[] = {
  {NULL}
};

static PyMemberDef Router_members[] = {
  {NULL}
};

static PyMethodDef Router_methods[] = {
  {"register", (PyCFunction)Router_register, METH_VARARGS, "register"},
  {"route", (PyCFunction)Router_route, METH_VARARGS, "route"},
  {"routes", (PyCFunction)Router_routes, METH_VARARGS, "routes"},
  {NULL}
};

PyTypeObject RouterType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "router.Router",                          /* tp_name */
  sizeof(Router),                           /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor)Router_dealloc,               /* tp_dealloc */
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
  "Router objects",                         /* tp_doc*/
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  0,                                        /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  Router_methods,                           /* tp_methods */
  Router_members,                           /* tp_members */
  Router_getseter,                          /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  (initproc)Router_init,                    /* tp_init */
  0,                                        /* tp_alloc */
  Router_new,                               /* tp_new */
};

static PyMethodDef router_module_methods[] = {
  {NULL}
};

PyObject *init_router(void) {
  PyObject* m;

  if (PyType_Ready(&RouterType) < 0) {
    return NULL;
  }

  if (PyType_Ready(&StaticRouterType) < 0) {
    return NULL;
  }

  if (PyType_Ready(&MVCRouterType) < 0) {
    return NULL;
  }

  if (PyType_Ready(&RESTRouterType) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.router", router_module_methods, "guava.router .");
  if (!m) {
    return NULL;
  }

  Py_INCREF(&RouterType);
  Py_INCREF(&StaticRouterType);
  Py_INCREF(&MVCRouterType);
  Py_INCREF(&RESTRouterType);

  PyModule_AddObject(m, "Router", (PyObject *)&RouterType);
  PyModule_AddObject(m, "StaticRouter", (PyObject *)&StaticRouterType);
  PyModule_AddObject(m, "MVCRouter", (PyObject *)&MVCRouterType);
  PyModule_AddObject(m, "RESTRouter", (PyObject *)&RESTRouterType);

  return m;
}
