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

PyObject *Handler_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  Handler *self;

  self = (Handler *)type->tp_alloc(type, 0);
  if (self) {
    self->handler = guava_handler_new();
  }

  return (PyObject *)self;
}

static void Handler_dealloc(Handler *self) {
  if (self->handler) {
    guava_handler_free(self->handler);
    self->handler = NULL;
  }

  self->ob_type->tp_free((PyObject *)self);
}

static int Handler_init(Handler *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"module", "cls", "action", "package", NULL};

  char *package = NULL;
  char *module = NULL;
  char *cls = NULL;
  char *action = NULL;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "sss|s",
                                   kwlist,
                                   &module,
                                   &cls,
                                   &action,
                                   &package)) {
    return -1;
  }

  self->handler->package = guava_string_new(package ? package : ".");
  self->handler->module = guava_string_new(module);
  self->handler->cls = guava_string_new(cls);
  self->handler->action = guava_string_new(action);

  guava_handler_mark_valid(self->handler);
  return 0;
}

static PyObject *Handler_get_package(Handler *self, void *closure) {
  guava_handler_t *handler = self->handler;
  return PyString_FromString(handler->package);
}

static int Handler_set_package(Handler *self, PyObject *value, void *closure) {
  guava_handler_t *handler = self->handler;
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the package attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The package attribute value must be a string");
    return -1;
  }

  guava_string_free(handler->package);

  handler->package = guava_string_new(PyString_AsString(value));
  return 0;
}

static PyObject *Handler_get_module(Handler *self, void *closure) {
  guava_handler_t *handler = self->handler;
  return PyString_FromString(handler->module);
}

static int Handler_set_module(Handler *self, PyObject *value, void *closure) {
  guava_handler_t *handler = self->handler;
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the module attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The module attribute value must be a string");
    return -1;
  }

  guava_string_free(handler->module);

  handler->module = guava_string_new(PyString_AsString(value));
  return 0;
}

static PyObject *Handler_get_cls(Handler *self, void *closure) {
  guava_handler_t *handler = self->handler;
  return PyString_FromString(handler->cls);
}

static int Handler_set_cls(Handler *self, PyObject *value, void *closure) {
  guava_handler_t *handler = self->handler;
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the cls attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The cls attribute value must be a string");
    return -1;
  }

  guava_string_free(handler->cls);

  handler->cls = guava_string_new(PyString_AsString(value));
  return 0;
}

static PyObject *Handler_get_action(Handler *self, void *closure) {
  guava_handler_t *handler = self->handler;
  return PyString_FromString(handler->action);
}

static int Handler_set_action(Handler *self, PyObject *value, void *closure) {
  guava_handler_t *handler = self->handler;
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the action attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The action attribute value must be a string");
    return -1;
  }

  guava_string_free(handler->action);

  handler->action = guava_string_new(PyString_AsString(value));
  return 0;
}

static PyObject *Handler_get_args(Handler *self, void *closure) {
  guava_handler_t *handler = self->handler;
  if (handler->args) {
    Py_INCREF(handler->args);
    return handler->args;
  }

  Py_RETURN_NONE;
}

static int Handler_set_args(Handler *self, PyObject *value, void *closure) {
  guava_handler_t *handler = self->handler;
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the args attribute");
    return -1;
  }

  if (!PyDict_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The args attribute value must be a string");
    return -1;
  }

  if (handler->args) {
    Py_DECREF(handler->args);
  }

  Py_INCREF(value);
  handler->args = value;

  return 0;
}

static PyObject *Handler_is_valid(Handler *self) {
  if (self->handler->flags & GUAVA_HANDLER_VALID) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

static PyObject *Handler_is_static(Handler *self) {
  if (self->handler->flags & GUAVA_HANDLER_STATIC) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

static PyObject *Handler_is_redirect(Handler *self) {
  if (self->handler->flags & GUAVA_HANDLER_REDIRECT) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

static PyGetSetDef Handler_getseter[] = {
  {"package", (getter)Handler_get_package, (setter)Handler_set_package, "package", NULL},
  {"module", (getter)Handler_get_module, (setter)Handler_set_module, "module", NULL},
  {"cls", (getter)Handler_get_cls, (setter)Handler_set_cls, "cls", NULL},
  {"action", (getter)Handler_get_action, (setter)Handler_set_action, "action", NULL},
  {"args", (getter)Handler_get_args, (setter)Handler_set_args, "args", NULL},
  {NULL}
};

static PyMemberDef Handler_members[] = {
  {NULL}
};

static PyMethodDef Handler_methods[] = {
  {"is_valid", (PyCFunction)Handler_is_valid, METH_NOARGS,
   "whether this handler is valid or not"},
  {"is_static", (PyCFunction)Handler_is_static, METH_NOARGS,
   "whether this handler is for getting a static file"},
  {"is_redirect", (PyCFunction)Handler_is_redirect, METH_NOARGS,
   "whether this handler is a URL redirect"},
  {NULL}
};


PyTypeObject HandlerType = {
  PyObject_HEAD_INIT(NULL)
  0,                            /* ob_size */
  "handler.Handler",            /* tp_name */
  sizeof(Handler),              /* tp_basicsize */
  0,                            /* tp_itemsize */
  (destructor)Handler_dealloc,  /* tp_dealloc */
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
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
  "Handler objects",            /* tp_doc*/
  0,                            /* tp_traverse */
  0,                            /* tp_clear */
  0,                            /* tp_richcompare */
  0,                            /* tp_weaklistoffset */
  0,                            /* tp_iter */
  0,                            /* tp_iternext */
  Handler_methods,              /* tp_methods */
  Handler_members,              /* tp_members */
  Handler_getseter,             /* tp_getset */
  0,                            /* tp_base */
  0,                            /* tp_dict */
  0,                            /* tp_descr_get */
  0,                            /* tp_descr_set */
  0,                            /* tp_dictoffset */
  (initproc)Handler_init,       /* tp_init */
  0,                            /* tp_alloc */
  Handler_new,                  /* tp_new */
};


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

static PyMethodDef handler_module_methods[] = {
  {NULL}
};

PyObject *init_handler(void) {
  PyObject* m;

  if (PyType_Ready(&HandlerType) < 0) {
    return NULL;
  }

  if (PyType_Ready(&RedirectHandlerType) < 0) {
    return NULL;
  }

  if (PyType_Ready(&StaticHandlerType) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.handler", handler_module_methods, "guava.handler .");

  if (!m) {
    return NULL;
  }

  Py_INCREF(&HandlerType);
  Py_INCREF(&RedirectHandlerType);
  Py_INCREF(&StaticHandlerType);

  PyModule_AddObject(m, "Handler", (PyObject *)&HandlerType);
  PyModule_AddObject(m, "RedirectHandler", (PyObject *)&RedirectHandlerType);
  PyModule_AddObject(m, "StaticHandler", (PyObject *)&StaticHandlerType);

  return m;
}
