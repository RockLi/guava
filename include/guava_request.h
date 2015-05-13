/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_REQUEST_H__
#define __GUAVA_REQUEST_H__

#include "guava_config.h"
#include "http_parser.h"
#include "guava_hashtable.h"
#include "guava_cookie.h"
#include "guava_url.h"

struct guava_request {
  guava_string_t      proto;
  uint16_t            proto_major;
  uint16_t            proto_minor;
  uint8_t             method;
  uint8_t             keep_alive;
  guava_url_t        *url;
  guava_hashtable_t  *headers;
  guava_cookie_t    **cookies;
  /* reserved for SESSION */
};

typedef struct {
  int8_t      code;
  const char *name;
} guava_request_method_t;

int
guava_request_on_message_begin(http_parser *parser);

int
guava_request_on_url(http_parser *parser,
                     const char  *buf,
                     size_t       len);

int
guava_request_on_header_field(http_parser *parser,
                              const char  *buf,
                              size_t       len);

int
guava_request_on_header_value(http_parser *parser,
                              const char  *buf,
                              size_t       len);

int
guava_request_on_headers_complete(http_parser *parser);

int
guava_request_on_body(http_parser *parser,
                      const char  *buf,
                      size_t       len);

int
guava_request_on_message_complete(http_parser *parser);

int8_t
guava_request_get_method(const char *s);

guava_request_t *
guava_request_new(void);

void
guava_request_free(guava_request_t *req);

void
guava_request_extract_from_url(guava_request_t *req);

char *
guava_request_parse_form_data(char           **data,
                              guava_string_t  *name,
                              guava_string_t  *value);

#endif /* !__GUAVA_REQUEST_H__ */
