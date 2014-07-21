/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_router/guava_router.h"
#include "guava_string.h"
#include "guava_request.h"
#include "guava_response.h"
#include "guava_handler.h"
#include "guava_session/guava_session.h"
#include "guava_memory.h"

guava_router_static_t *guava_router_static_new(void) {
  guava_router_static_t *router = (guava_router_static_t *)guava_malloc(sizeof(guava_router_static_t));
  if (!router) {
    return NULL;
  }

  router->route.mount_point = guava_string_new("/static");
  router->route.package = guava_string_new(".");
  router->route.type = GUAVA_ROUTER_STATIC;
  router->route.session_store = NULL;
  router->route.routes = NULL;
  router->directory = guava_string_new("./static");
  router->allow_index = GUAVA_FALSE;

  return router;
}

void guava_router_static_free(guava_router_static_t *router) {
  if (!router) {
    return;
  }

  if (router->route.mount_point) {
    guava_string_free(router->route.mount_point);
  }

  if (router->route.package) {
    guava_string_free(router->route.package);
  }

  if (router->route.session_store) {
    guava_session_store_free(router->route.session_store);
  }

  if (router->route.routes) {
    Py_DECREF(router->route.routes);
  }

  if (router->directory) {
    guava_string_free(router->directory);
  }

  guava_free(router);
}

void guava_router_static_set_directory(guava_router_static_t *router, const char *directory) {
  if (!router || !directory) {
    return;
  }

  if (router->directory) {
    guava_string_free(router->directory);
  }

  router->directory = guava_string_new(directory);
}

void guava_router_static_set_allow_index(guava_router_static_t *router, const guava_bool_t allow_index) {
  if (!router) {
    return;
  }

  router->allow_index = allow_index;
}

void guava_router_static_route(guava_router_static_t *router, guava_request_t *req, guava_handler_t *handler) {
  if (!router || !req) {
    return;
  }

  handler->flags |= GUAVA_HANDLER_VALID;
  handler->flags |= GUAVA_HANDLER_STATIC;
}
