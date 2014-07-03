/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_handler.h"
#include "guava_string.h"
#include "guava_memory.h"

guava_handler_t *guava_handler_new() {
  guava_handler_t *handler = (guava_handler_t *)guava_calloc(1, sizeof(guava_handler_t));
  if (!handler) {
    return NULL;
  }

  return handler;
}

void guava_handler_free(guava_handler_t *handler) {
  if (handler) {
    guava_handler_deinit(handler);
    guava_free(handler);
  }
}

void guava_handler_init(guava_handler_t *handler) {
  if (!handler) {
    return;
  }

  handler->flags = 0;
  handler->package = NULL;
  handler->module = NULL;
  handler->cls = NULL;
  handler->action = NULL;
  handler->args = NULL;
}

void guava_handler_deinit(guava_handler_t *handler) {
  if (!handler) {
    return;
  }
  handler->flags = 0;

  if (handler->package) {
    guava_string_free(handler->package);
  }

  if (handler->module) {
    guava_string_free(handler->module);
  }

  if (handler->cls) {
    guava_string_free(handler->cls);
  }

  if (handler->action) {
    guava_string_free(handler->action);
  }

  if (handler->args) {
    Py_DECREF(handler->args);
    handler->args = NULL;
  }
}

guava_bool_t guava_handler_is_valid(guava_handler_t *handler) {
  return handler->flags & GUAVA_HANDLER_VALID ? GUAVA_TRUE : GUAVA_FALSE;
}

void guava_handler_mark_valid(guava_handler_t *handler) {
  handler->flags |= GUAVA_HANDLER_VALID;
}

void guava_handler_print(guava_handler_t *handler) {
  fprintf(stderr, "package: %s\n", handler->package);
  fprintf(stderr, "module: %s\n", handler->module);
  fprintf(stderr, "cls: %s\n", handler->cls);
  fprintf(stderr, "action: %s\n", handler->action);
}

void guava_handler_copy(guava_handler_t *src, guava_handler_t *dst) {
  dst->flags = src->flags;
  dst->package = src->package ? guava_string_new(src->package) : NULL;
  dst->module = src->module ? guava_string_new(src->module) : NULL;
  dst->cls = src->cls ? guava_string_new(src->cls) : NULL;
  dst->action = src->action ? guava_string_new(src->action) : NULL;
  dst->router = src->router;
  dst->args = src->args;
  Py_XINCREF(dst->args);
}
