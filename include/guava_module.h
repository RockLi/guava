/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_MODULE_H__
#define __GUAVA_MODULE_H__

#include "guava.h"
#include "guava_router/guava_router.h"
#include "guava_handler.h"
#include "guava_request.h"
#include "guava_url.h"
#include "guava_cookie.h"

typedef struct {
  PyObject_HEAD

  guava_server_t *server;

  char *ip;
  int port;
  int backlog;
  char auto_reload;
} Server;

typedef struct {
  PyObject_HEAD

  guava_url_t *url;
} Py_URL;

typedef struct {
  PyObject_HEAD

  guava_cookie_t *cookie;
} Py_Cookie;

typedef struct {
  PyObject_HEAD

  guava_router_t *router;
} Router;

typedef struct {
  PyObject_HEAD

  guava_handler_t *handler;
} Handler;

typedef struct {
  PyObject_HEAD

  guava_handler_t *handler;
} RedirectHandler;

typedef struct {
  PyObject_HEAD

  guava_handler_t *handler;
} StaticHandler;

typedef struct {
  PyObject_HEAD

  guava_request_t *req;
} Request;

typedef struct {
  PyObject_HEAD

  guava_response_t *res;
} Response;

typedef struct {
  PyObject_HEAD

  PyObject         *req;
  PyObject         *res;

  PyObject         *SESSION;
  guava_router_t   *router;
} Controller;


extern PyTypeObject HandlerType;

extern PyTypeObject RedirectHandlerType;

extern PyTypeObject StaticHandlerType;

extern PyTypeObject RouterType;

extern PyTypeObject RequestType;

extern PyTypeObject ControllerType;

extern PyTypeObject PyType_Cookie;

extern PyTypeObject PyType_URL;

extern PyObject *Handler_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

#define MODULE_SETTER_CHECK_PROPERTY_DELETE(msg)        \
  do {                                                  \
    if (value == NULL) {                                \
      PyErr_SetString(PyExc_TypeError, (msg));          \
      return -1;                                        \
    }                                                   \
  } while(0)


#endif /* !__GUAVA_MODULE_H__ */
