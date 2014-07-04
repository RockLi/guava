/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_server.h"
#include "guava_module.h"
#include "guava_router/guava_router.h"
#include "guava_module_router.h"
#include "guava_memory.h"

typedef struct {
  PyObject_HEAD

  guava_server_t *server;

  char *ip;
  int port;
  int backlog;
  char auto_reload;
} Server;

static PyObject *Server_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  Server *self;

  self = (Server *)type->tp_alloc(type, 0);
  if (self) {
    self->server = guava_server_new();
    self->ip = GUAVA_SERVER_DEFAULT_LISTEN_IP;
    self->port = GUAVA_SERVER_DEFAULT_LISTEN_PORT;
    self->backlog = GUAVA_SERVER_DEFAULT_LISTEN_BACKLOG;
    self->auto_reload = 0;
  }

  return (PyObject *)self;
}

static void Server_dealloc(Server *self) {
  guava_server_free(self->server);

  self->ob_type->tp_free((PyObject *)self);
}

static int Server_init(Server *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"ip", "port", "backlog", "auto_reload", NULL};

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "|siib",
                                   kwlist,
                                   &self->ip,
                                   &self->port,
                                   &self->backlog,
                                   &self->auto_reload)) {
    return -1;
  }

  return 0;
}

static PyObject *Server_serve(Server *self) {
  guava_server_start(self->server, self->ip, self->port, self->backlog);
  Py_RETURN_NONE;
}

static PyObject *Server_add_router(Server *self, PyObject *args) {
  Router *router;

  Py_ssize_t size = PyTuple_Size(args);

  if (size == 0) {
    PyErr_SetString(PyExc_TypeError, "at least give one router");
    return NULL;
  }

  for (Py_ssize_t i = 0; i < size; ++i) {
    router = (Router *)PyTuple_GetItem(args, i);
    Py_INCREF(router);

    guava_server_add_router(self->server, router);
  }

  Py_RETURN_NONE;
}

static PyObject *Server_repr(Server *self) {
  return PyString_FromFormat("Server listen(%s:%d), backlog(%d), auto_reload(%s)",
                             self->ip,
                             self->port,
                             self->backlog,
                             self->auto_reload ? "TRUE" : "FALSE");
}

static PyMemberDef Server_members[] = {
  {"ip", T_STRING, offsetof(Server, ip), 0, "ip"},
  {"port", T_INT, offsetof(Server, port), 0, "port"},
  {"backlog", T_INT, offsetof(Server, backlog), 0, "backlog"},
  {"auto_reload", T_BOOL, offsetof(Server, auto_reload), 0, "auto reload"},
  {NULL}
};

static PyMethodDef Server_methods[] = {
  {"add_router", (PyCFunction)Server_add_router, METH_VARARGS, "add one router"},
  {"serve", (PyCFunction)Server_serve, METH_NOARGS, "start the web server"},
  {NULL}
};

static PyTypeObject ServerType = {
  PyObject_HEAD_INIT(NULL)
  0,                          /* ob_size */
  "server.Server",            /* tp_name */
  sizeof(Server),             /* tp_basicsize */
  0,                          /* tp_itemsize */
  (destructor)Server_dealloc, /* tp_dealloc */
  0,                          /* tp_print */
  0,                          /* tp_getattr */
  0,                          /* tp_setattr */
  0,                          /* tp_compare */
  (reprfunc)Server_repr,      /* tp_repr */
  0,                          /* tp_as_number */
  0,                          /* tp_as_sequence */
  0,                          /* tp_as_mapping */
  0,                          /* tp_hash */
  0,                          /* tp_call */
  0,                          /* tp_str */
  0,                          /* tp_getattro */
  0,                          /* tp_setattro */
  0,                          /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,         /* tp_flags */
  "Server objects",           /* tp_doc*/
  0,                          /* tp_traverse */
  0,                          /* tp_clear */
  0,                          /* tp_richcompare */
  0,                          /* tp_weaklistoffset */
  0,                          /* tp_iter */
  0,                          /* tp_iternext */
  Server_methods,             /* tp_methods */
  Server_members,             /* tp_members */
  0,                          /* tp_getset */
  0,                          /* tp_base */
  0,                          /* tp_dict */
  0,                          /* tp_descr_get */
  0,                          /* tp_descr_set */
  0,                          /* tp_dictoffset */
  (initproc)Server_init,      /* tp_init */
  0,                          /* tp_alloc */
  Server_new,                 /* tp_new */
};


static PyObject *server_start_static_server(PyObject *self, PyObject *args, PyObject *kwds) {
  char *path = ".";
  guava_bool_t allow_index = GUAVA_TRUE;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "|sB",
                                   &path,
                                   &allow_index)) {
    return NULL;
  }

  guava_server_t *server = guava_server_new();

  StaticRouter *router = (StaticRouter *)PyObject_New(StaticRouter, &StaticRouterType);
  router->router.router = (guava_router_t *)guava_router_static_new();
  guava_router_set_mount_point(router->router.router, "/");
  guava_router_static_set_directory((guava_router_static_t *)router->router.router, path);
  guava_router_static_set_allow_index((guava_router_static_t *)router->router.router, allow_index);

  guava_server_add_router(server, (Router *)router);
  Py_DECREF(router);

  guava_server_start(server, GUAVA_SERVER_DEFAULT_LISTEN_IP, GUAVA_SERVER_DEFAULT_LISTEN_PORT, GUAVA_SERVER_DEFAULT_LISTEN_BACKLOG);

  Py_RETURN_NONE;
}

static PyMethodDef server_module_methods[] = {
  {"start_static_server", (PyCFunction)server_start_static_server, METH_VARARGS | METH_KEYWORDS, "add one router"},
  {NULL}
};

PyObject *init_server(void) {
  PyObject* m;

  if (PyType_Ready(&ServerType) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.server", server_module_methods, "guava.server .");

  if (!m) {
    return NULL;
  }

  Py_INCREF(&ServerType);

  PyModule_AddObject(m, "Server", (PyObject *)&ServerType);

  return m;
}
