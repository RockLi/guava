/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_STRING_H__
#define __GUAVA_STRING_H__

#include "guava.h"

#define GUAVA_STRING_CONST 0x01

static inline size_t guava_string_len(const guava_string_t gs) {
  guava_string_header_t *h = (guava_string_header_t *)((char *)gs - sizeof(guava_string_header_t));
  return h->len;
}

guava_string_t guava_string_new(const char *init);

guava_string_t guava_string_new_size(const char *init, size_t len);

guava_string_t guava_string_append(const guava_string_t gs, const guava_string_t gs2);

guava_string_t guava_string_append_raw(const guava_string_t gs, const char *s);

guava_string_t guava_string_append_raw_size(const guava_string_t gs, const char *s, size_t len);

guava_bool_t guava_string_starts_with(const guava_string_t s1, const guava_string_t s2);

guava_bool_t guava_string_equal_raw(const guava_string_t s, const char *s2);

size_t guava_string_common_string_count_from_start(const guava_string_t s, const guava_string_t s2);

void guava_string_free(const guava_string_t gs);

#endif /* !__GUAVA_STRING_H__ */
