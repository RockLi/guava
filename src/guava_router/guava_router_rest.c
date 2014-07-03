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

guava_router_rest_t *guava_router_rest_new(void) {
  guava_router_rest_t *router = (guava_router_rest_t *)guava_calloc(1, sizeof(guava_router_rest_t));
  if (!router) {
    return NULL;
  }

  router->route.mount_point = guava_string_new("/");
  router->route.package = guava_string_new(".");
  router->route.type = GUAVA_ROUTER_REST;
  router->route.session_store = NULL;
  router->route.routes = NULL;

  return router;
}

void guava_router_rest_free(guava_router_rest_t *router) {
  if (router->route.session_store) {
    guava_session_store_free(router->route.session_store);
  }

  if (router->route.routes) {
    Py_DECREF(router->route.routes);
  }

  if (router) {
    guava_free(router);
  }
}

void guava_router_rest_route(guava_router_rest_t *router, guava_request_t *req, guava_handler_t *handler) {
  if (!router || !req || !handler) {
    return;
  }

  const char *ptr = strstr(req->path, router->route.mount_point);
  if (!ptr) {
    return;
  }

  ptr += guava_string_len(router->route.mount_point);
  if (!ptr) {
    return;
  }

  /* try to split the path and get the resource */
  const char *next_slash = strchr(ptr, '/');

  guava_string_t resource = NULL;
  guava_string_t item_id = NULL;

  if (next_slash != ptr) {
    resource = guava_string_new_size(ptr, next_slash - ptr);
  }

  ptr = next_slash + 1;

  if (ptr) {
    next_slash = strchr(ptr, '/');
    if (next_slash) {
      item_id = guava_string_new_size(ptr, next_slash - ptr);
    } else if (strlen(ptr)) {
      item_id = guava_string_new(ptr);
    }
  }

  handler->module = resource;

  char s[1024];
  snprintf(s, sizeof(s), "%c%sController", toupper(resource[0]), resource+1);
  handler->cls = guava_string_new(s);

  switch(req->method) {
  case HTTP_GET: {
    /* Get all items or one specific item */
    if (!item_id) {
      handler->action = guava_string_new("get_all");
    } else {
      handler->action = guava_string_new("get_one");
    }
    break;
  }

  case HTTP_POST: {
    /* Create new item */

    if (item_id) {
      handler->flags |= GUAVA_HANDLER_404;
      break;
    }

    handler->action = guava_string_new("create_one");

    break;
  }

  case HTTP_PUT: {
    /* PUT should be put to the root of resource */
    if (!item_id) {
      handler->flags |= GUAVA_HANDLER_404;
      break;
    }

    handler->action = guava_string_new("update_one");
    break;
  }

  case HTTP_DELETE: {
    if (!item_id) {
      handler->flags |= GUAVA_HANDLER_404;
      break;
    }

    handler->action = guava_string_new("delete_one");
    break;
  }

  default: {
    handler->flags |= GUAVA_HANDLER_404;
    break;
  }
  }

  if (!(handler->flags & GUAVA_HANDLER_404)) {
    handler->package = guava_string_new(router->route.package);
    guava_handler_mark_valid(handler);
  }
}
