/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_REQUEST_H__
#define __GUAVA_REQUEST_H__

#include "guava.h"
#include "http_parser.h"

typedef struct {
  int8_t code;
  const char *name;
} guava_request_method_t;

int8_t guava_request_get_method(const char *s);

int guava_request_on_message_begin(http_parser *parser);

int guava_request_on_url(http_parser *parser, const char *buf, size_t len);

int guava_request_on_header_field(http_parser *parser, const char *buf, size_t len);

int guava_request_on_header_value(http_parser *parser, const char *buf, size_t len);

int guava_request_on_headers_complete(http_parser *parser);

int guava_request_on_body(http_parser *parser, const char *buf, size_t len);

int guava_request_on_message_complete(http_parser *parser);

guava_request_t *guava_request_new(void);

void guava_request_free(guava_request_t *req);

void guava_request_extract_from_url(guava_request_t *req);

char *guava_request_parse_form_data(char **data, guava_string_t *name, guava_string_t *value);

#endif /* !__GUAVA_REQUEST_H__ */
