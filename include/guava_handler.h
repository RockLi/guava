/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_HANDLER_H__
#define __GUAVA_HANDLER_H__

#include "guava_config.h"
#include "guava.h"

#define GUAVA_HANDLER_VALID 1<<0
#define GUAVA_HANDLER_404 1<<1
#define GUAVA_HANDLER_REDIRECT 1<<2
#define GUAVA_HANDLER_STATIC 1<<3

struct guava_handler{
  uint32_t        flags;
  guava_string_t  package;
  guava_string_t  module;
  guava_string_t  cls;
  guava_string_t      action;
  struct guava_router *router;
  PyObject       *args;
};

struct guava_handler *
guava_handler_new(void);

void
guava_handler_free(struct guava_handler *handler);

void
guava_handler_init(struct guava_handler *handler);

void
guava_handler_deinit(struct guava_handler *handler);

guava_bool_t
guava_handler_is_valid(struct guava_handler *handler);

void guava_handler_static(struct guava_router   *router,
                          struct guava_conn     *conn,
                          struct guava_request  *req,
                          struct guava_response *resp,
                          uv_write_cb            on_write,
                          uv_fs_cb               on_sendfile);

void
guava_handler_mark_valid(struct guava_handler *handler);

void
guava_handler_print(struct guava_handler *handler);

void
guava_handler_copy(struct guava_handler *src,
                   struct guava_handler *dst);

#endif /* __GUAVA_HANDLER_H__ */
