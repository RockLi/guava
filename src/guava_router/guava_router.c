/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_router/guava_router.h"
#include "guava_string.h"
#include "guava_request.h"
#include "guava_response.h"
#include "guava_module.h"
#include "guava_memory.h"

guava_router_t *guava_router_new(void) {
  guava_router_t *router = (guava_router_t *)guava_calloc(1, sizeof(guava_router_t));
  if (router) {
    router->mount_point = NULL;
    router->package = NULL;
    router->type = GUAVA_ROUTER_CUSTOM;
    router->session_store = NULL;
    router->routes = PyDict_New();
  }

  return router;
}

void guava_router_free(guava_router_t *router) {
  if (!router) {
    return;
  }

  if (router->routes) {
    Py_DECREF(router->routes);
  }
}

void guava_router_set_mount_point(guava_router_t *router, const char *mount_point) {
  if (!router || !mount_point) {
    return;
  }

  if (router->mount_point) {
    guava_string_free(router->mount_point);
  }

  router->mount_point = guava_string_new(mount_point);
  if (router->mount_point[guava_string_len(router->mount_point)-1] != '/') {
    router->mount_point = guava_string_append_raw(router->mount_point, "/");
  }
}

void guava_router_set_package(guava_router_t *router, const char *package) {
  if (!router || !package) {
    return;
  }

  router->package = guava_string_new(package);
}

void guava_router_set_session_store(guava_router_t *router, guava_session_store_t *store) {
  if (!router || !store) {
    return;
  }

  router->session_store = store;
}

void guava_router_set_routes(guava_router_t *router, PyObject *routes) {
  if (!router || !routes) {
    return;
  }

  router->routes = routes;
}

PyObject *guava_router_get_best_matched_router(PyObject *routers, PyObject *request) {
  Router *router = NULL;
  Py_ssize_t size = PyList_Size(routers);
  size_t best_length = 0;

  for (Py_ssize_t i = 0; i < size; ++i) {
      Router *r = (Router *)PyList_GetItem(routers, i);
      if (r->router->type == GUAVA_ROUTER_CUSTOM) { /* Skip all custom routers */
        continue;
      }
      size_t x = guava_string_common_string_count_from_start(r->router->mount_point, ((Request *)request)->req->url);
      if (x >= best_length) {
        best_length = x;
        router = r;
      }
  }

  return (PyObject *)router;
}
