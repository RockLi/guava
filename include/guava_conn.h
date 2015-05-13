/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_CONN_H__
#define __GUAVA_CONN_H__

#include "guava_config.h"

struct guava_conn {
  uv_tcp_t              stream;
  http_parser           parser;
  http_parser_settings  parser_settings;
  uv_write_t            write_req;
  guava_server_t       *server;
  guava_bool_t          keep_alive;
  guava_request_t      *request;
  guava_string_t        auxiliary_current_header;
  guava_bool_t          auxiliary_last_was_header;
};

guava_conn_t *guava_conn_new(void);

void guava_conn_free(guava_conn_t *conn);

#endif /* !__GUAVA_CONN_H__ */
