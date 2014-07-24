/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_RESPONSE_H__
#define __GUAVA_RESPONSE_H__

#include "guava.h"

typedef struct {
  int         code;
  const char *desc;
} guava_status_code_t;

guava_response_t *guava_response_new(void);

void guava_response_free(guava_response_t *resp);

void guava_response_set_conn(guava_response_t *resp, guava_conn_t *conn);

void guava_response_set_version(guava_response_t *resp, uint16_t major, uint16_t minor);

void guava_response_set_status_code(guava_response_t *resp, uint16_t status_code);

void guava_response_set_header(guava_response_t *resp, const char *key, const char *value);

void guava_response_set_cookie(guava_response_t *resp, const char *key, PyObject *value);

void guava_response_set_data(guava_response_t *resp, guava_string_t data);

void guava_response_write_data(guava_response_t *resp, const char *data);

guava_string_t guava_response_serialize(guava_response_t *resp);

void guava_response_send(guava_response_t *resp, uv_write_cb cb);

void guava_response_404(guava_response_t *resp, void *closure);

void guava_response_500(guava_response_t *resp, void *closure);

void guava_response_302(guava_response_t *resp, void *closure);

#endif /* !__GUAVA_RESPONSE_H__ */
