/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_module.h"
#include "guava_memory.h"

extern PyObject *init_request(void);

extern PyObject *init_server(void);

extern PyObject *init_router(void);

extern PyObject *init_handler(void);

extern PyObject *init_controller(void);

extern PyObject *init_session(void);

extern PyObject *init_cookie(void);

extern PyObject *init_url_module(void);

guava_bool_t register_module(PyObject *package, const char *name, PyObject *module) {
  if (!module) {
    return GUAVA_FALSE;
  }

  Py_INCREF(module);

  if (PyModule_AddObject(package, name, module)) {
    Py_DECREF(module);
    return GUAVA_FALSE;
  }

  return GUAVA_TRUE;
}

PyObject *init_guava(void) {
  PyObject *guava_module      = NULL;
  PyObject *request_module    = NULL;
  PyObject *server_module     = NULL;
  PyObject *handler_module    = NULL;
  PyObject *controller_module = NULL;
  PyObject *router_module     = NULL;
  PyObject *session_module    = NULL;
  PyObject *cookie_module     = NULL;
  PyObject *url_module        = NULL;

  PyEval_InitThreads();

  guava_module = Py_InitModule("guava", NULL);

  request_module = init_request();
  if (!register_module(guava_module, "request", request_module)) {
    return NULL;
  }

  server_module = init_server();
  if (!register_module(guava_module, "server", server_module)) {
    return NULL;
  }

  handler_module = init_handler();
  if (!register_module(guava_module, "handler", handler_module)) {
    return NULL;
  }

  router_module = init_router();
  if (!register_module(guava_module, "router", router_module)) {
    return NULL;
  }

  controller_module = init_controller();
  if (!register_module(guava_module, "controller", controller_module)) {
    return NULL;
  }

  session_module = init_session();
  if (!register_module(guava_module, "session", session_module)) {
    return NULL;
  }

  cookie_module = init_cookie();
  if (!register_module(guava_module, "cookie", cookie_module)) {
    return NULL;
  }

  url_module = init_url_module();
  if (!register_module(guava_module, "url", url_module)) {
    return NULL;
  }

  PyModule_AddStringConstant(guava_module, "version", GUAVA_VERSION);

  return guava_module;
}

PyMODINIT_FUNC initguava(void) {
  init_guava();
}
