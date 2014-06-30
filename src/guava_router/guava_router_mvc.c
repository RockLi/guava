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

guava_router_mvc_t *guava_router_mvc_new(void) {
  guava_router_mvc_t *router = (guava_router_mvc_t *)malloc(sizeof(guava_router_mvc_t));
  if (!router) {
    return NULL;
  }

  router->route.mount_point = guava_string_new("/");
  router->route.package = guava_string_new(".");
  router->route.type = GUAVA_ROUTER_MVC;
  router->route.session_store = NULL;
  router->route.routes = NULL;

  return router;
}

void guava_router_mvc_free(guava_router_mvc_t *router) {
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

  free(router);
}

void guava_router_mvc_route(guava_router_mvc_t *router, guava_request_t *req, guava_handler_t *handler) {
  if (!router || !req || !handler) {
    return;
  }

  char *ptr = strstr(req->path, router->route.mount_point);
  if (!ptr) {
    return;
  }

  ptr += guava_string_len(router->route.mount_point);

  const char *sep = "/";

  char *word, *save;

  guava_string_t module = NULL;
  guava_string_t cls = NULL;
  guava_string_t action = NULL;

  size_t idx = 0;
  for (word = strtok_r(ptr, sep, &save); word; word = strtok_r(NULL, sep, &save), ++idx) {
    if (idx == 0) {
      module = guava_string_new(word);
      char s[1024];
      snprintf(s, sizeof(s), "%c%sController", toupper(word[0]), word+1);
      cls = guava_string_new(s);
    } else if (idx == 1) {
      action = guava_string_new(word);
    }
  }

  if (!module) {
    module = guava_string_new("index");
  }
  if (!cls) {
    cls = guava_string_new("IndexController");
  }
  if (!action) {
    action = guava_string_new("index");
  }

  handler->package = guava_string_new(".");
  handler->module = module;
  handler->cls = cls;
  handler->action = action;
  guava_handler_mark_valid(handler);
}
