/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_HANDLER_H__
#define __GUAVA_HANDLER_H__

#include "guava.h"

#define GUAVA_HANDLER_VALID 1<<0
#define GUAVA_HANDLER_404 1<<1
#define GUAVA_HANDLER_REDIRECT 1<<2

guava_handler_t *guava_handler_new(void);

void guava_handler_free(guava_handler_t *handler);

void guava_handler_init(guava_handler_t *handler);

void guava_handler_deinit(guava_handler_t *handler);


guava_bool_t guava_handler_is_valid(guava_handler_t *handler);

void guava_handler_static(guava_router_t *router,
                          guava_conn_t *conn,
                          guava_request_t *req,
                          guava_response_t *resp,
                          uv_write_cb on_write,
                          uv_fs_cb on_sendfile);

void guava_handler_mark_valid(guava_handler_t *handler);

void guava_handler_print(guava_handler_t *handler);

void guava_handler_copy(guava_handler_t *src, guava_handler_t *dst);

#endif /* __GUAVA_HANDLER_H__ */
