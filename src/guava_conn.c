/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_conn.h"
#include "guava_string.h"
#include "guava_request.h"
#include "guava_memory.h"

guava_conn_t *guava_conn_new() {
  guava_conn_t *conn = (guava_conn_t *)guava_calloc(1, sizeof(guava_conn_t));
  if (!conn) {
    return NULL;
  }

  conn->parser_settings.on_message_begin = guava_request_on_message_begin;
  conn->parser_settings.on_url = guava_request_on_url;
  conn->parser_settings.on_header_field = guava_request_on_header_field;
  conn->parser_settings.on_header_value = guava_request_on_header_value;
  conn->parser_settings.on_headers_complete = guava_request_on_headers_complete;
  conn->parser_settings.on_body = guava_request_on_body;
  conn->parser_settings.on_message_complete = guava_request_on_message_complete;

  conn->server = NULL;
  conn->keep_alive = GUAVA_FALSE;
  conn->request = NULL;

  return conn;
}

void guava_conn_free(guava_conn_t *conn) {
  Py_XDECREF(conn->request);

  if (conn->auxiliary_current_header) {
    guava_string_free(conn->auxiliary_current_header);
  }

  guava_free(conn);
}
