/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_SERVER_H__
#define __GUAVA_SERVER_H__

#include "guava.h"
#include "guava_module.h"

guava_server_t *guava_server_new(void);

void guava_server_free(guava_server_t *server);

void guava_server_on_conn(uv_stream_t *stream, int status);

void guava_server_on_alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);

void guava_server_on_read(uv_stream_t *stream, ssize_t read, const uv_buf_t *buf);

void guava_server_on_close(uv_handle_t *handle);

void guava_server_start(guava_server_t *server);

void guava_server_add_router(guava_server_t *server, Router *router);

#endif /* !__GUAVA_SERVER_H__ */
