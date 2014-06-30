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

typedef struct {
  PyObject_HEAD

  guava_router_t *router;
} Router;

typedef struct {
  PyObject_HEAD

  guava_session_store_t *store;
} SessionStore;

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

  guava_request_t *req;
} Request;

typedef struct {
  PyObject_HEAD

  guava_response_t *resp;
} Response;

typedef struct {
  PyObject_HEAD

  guava_response_t *resp;
  PyObject *owned_resp;

  PyObject *SESSION;
  guava_router_t *router;
} Controller;

extern PyTypeObject HandlerType;

extern PyTypeObject RouterType;

extern PyTypeObject RequestType;

extern PyTypeObject ControllerType;

extern PyObject *Handler_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

#endif /* !__GUAVA_MODULE_H__ */
