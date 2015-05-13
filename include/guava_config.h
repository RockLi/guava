/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_CONFIG_H__
#define __GUAVA_CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <uv.h>
#include "http_parser.h"

#define container_of(ptr, type, member)                 \
  ((type *) ((char *) (ptr) - offsetof(type, member)))

#ifndef GUAVA_TRUE
#define GUAVA_TRUE 1
#endif

#ifndef GUAVA_FALSE
#define GUAVA_FALSE 0
#endif

#ifndef MAXPATH
#define MAXPATH 1024
#endif

typedef uint8_t guava_bool_t;

typedef struct guava_string_header guava_string_header_t;
typedef char *guava_string_t;

typedef struct guava_request guava_request_t;

typedef struct guava_response guava_response_t;

typedef struct guava_router guava_router_t;

typedef struct guava_server guava_server_t;

typedef struct guava_handler guava_handler_t;

typedef struct guava_conn guava_conn_t;

#endif /* !__GUAVA_CONFIG_H__ */
