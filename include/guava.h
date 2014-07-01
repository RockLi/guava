/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_H__
#define __GUAVA_H__

#include <Python.h>
#include <structmember.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <uv.h>
#include "http_parser.h"

#define GUAVA_VERSION "0.01DP"

#define container_of(ptr, type, member)                 \
  ((type *) ((char *) (ptr) - offsetof(type, member)))

#define SERVER_NAME "Guava-0.01DP"

typedef uint8_t guava_bool_t;

#ifndef GUAVA_TRUE
#define GUAVA_TRUE 1
#endif

#ifndef GUAVA_FALSE
#define GUAVA_FALSE 0
#endif

#ifndef MAXPATH
#define MAXPATH 1024
#endif

typedef struct {
  size_t len;
  char   data[0];
} guava_string_header_t;

typedef char *guava_string_t;

typedef enum {
  GUAVA_ROUTER_STATIC,
  GUAVA_ROUTER_REST,
  GUAVA_ROUTER_MVC,
  GUAVA_ROUTER_CUSTOM
} guava_router_type_t;

typedef enum {
  GUAVA_SESSION_STORE_INMEM,
  GUAVA_SESSION_STORE_FILE,
  GUAVA_SESSION_STORE_CUSTOM
} guava_session_store_type_t;

typedef struct {
  const char                *name;            /* session name passed to the client */
  int                        gc_time;         /* recycle the session item while exceeds this time in second */
  int                        cookie_time;     /* the cookie expired time */
  const char                *cookie_path;
  const char                *cookie_domain;
  guava_bool_t               cookie_secure;
  guava_bool_t               cookie_httponly;
  guava_session_store_type_t type;
} guava_session_store_t;

typedef struct {
  guava_router_type_t    type;
  guava_string_t         mount_point;
  guava_string_t         package;
  guava_session_store_t *session_store;
  PyObject              *routes; /* Special routes for overriding the default actions */
} guava_router_t;

typedef struct {
  uint32_t        flags;
  guava_string_t  package;
  guava_string_t  module;
  guava_string_t  cls;
  guava_string_t  action;
  guava_router_t *router;
  PyObject       *args;
} guava_handler_t;

typedef struct {
  uv_loop_t    loop;
  uv_tcp_t     server;
  uv_signal_t  signal;
  PyObject    *routers;
} guava_server_t;

typedef struct {
  uint16_t        major;
  uint16_t        minor;
  uint8_t         method;
  uint8_t         keep_alive;
  PyObject       *headers;
  guava_string_t  url;
  guava_string_t  body;
  guava_string_t  path;
  guava_string_t  host;
  PyObject       *GET; /* Dict for storing the get parameters */
  PyObject       *POST; /* Dict for storing the post parameters */
  PyObject       *COOKIES;
} guava_request_t;

typedef struct {
  uv_tcp_t              stream;
  http_parser           parser;
  http_parser_settings  parser_settings;
  uv_write_t            write_req;
  PyObject             *request;
  guava_server_t       *server;
  uint8_t               keep_alive;
  guava_string_t        auxiliary_current_header;
  uint8_t               auxiliary_last_was_header;
} guava_conn_t;

typedef struct {
  uint16_t        major;
  uint16_t        minor;
  uint16_t        status_code;
  PyObject       *headers;
  PyObject       *cookies;
  guava_conn_t   *conn;
  guava_string_t  data;
  guava_string_t  serialized_data;
} guava_response_t;

typedef struct {
  guava_string_t name;
  guava_string_t value;
  guava_string_t domain;
  guava_string_t path;
  int            expired;
  int            max_age;
  guava_bool_t   secure;
  guava_bool_t   httponly;
} guava_cookie_t;

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

#endif /* !__GUAVA_H__ */
