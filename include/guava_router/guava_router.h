/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_ROUTER_H__
#define __GUAVA_ROUTER_H__

#include "guava.h"

typedef struct {
  guava_router_t route;
  guava_string_t directory;
  guava_bool_t   allow_index;
} guava_router_static_t;

typedef struct {
  guava_router_t route;
} guava_router_mvc_t;

typedef struct {
  guava_router_t route;
} guava_router_rest_t;

typedef struct {
  guava_router_t  route;
  PyObject       *routes;
} guava_router_custom_t;

guava_router_t *guava_router_new(void);
void guava_router_free(guava_router_t *router);

void guava_router_set_mount_point(guava_router_t *router, const char *mount_point);
void guava_router_set_package(guava_router_t *router, const char *package);
void guava_router_set_session_store(guava_router_t *router, guava_session_store_t *store);
void guava_router_set_routes(guava_router_t *router, PyObject *routes);

guava_router_static_t *guava_router_static_new(void);
void guava_router_static_free(guava_router_static_t *router);
void guava_router_static_set_directory(guava_router_static_t *router, const char *directory);
void guava_router_static_set_allow_index(guava_router_static_t *router, const guava_bool_t allow_index);
void guava_router_static_route(guava_router_static_t *router, guava_request_t *req, guava_handler_t *handler);

guava_router_mvc_t *guava_router_mvc_new(void);
void guava_router_mvc_free(guava_router_mvc_t *router);
void guava_router_mvc_route(guava_router_mvc_t *router, guava_request_t *req, guava_handler_t *handler);

guava_router_rest_t *guava_router_rest_new(void);
void guava_router_rest_free(guava_router_rest_t *router);
void guava_router_rest_route(guava_router_rest_t *router, guava_request_t *req, guava_handler_t *handler);

PyObject *guava_router_get_best_matched_router(PyObject *routers, PyObject *request);

#endif /* !__GUAVA_ROUTER_H__ */
