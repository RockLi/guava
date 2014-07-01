/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_string.h"
#include <string.h>

guava_string_t guava_string_new(const char *init) {
  return guava_string_new_size(init, init ? strlen(init) : 0);
}

guava_string_t guava_string_new_size(const char *init, size_t len) {
  guava_string_header_t *h;

  if (init) {
    h = malloc(sizeof(guava_string_header_t) + len + 1);
  } else {
    h = calloc(1, sizeof(guava_string_header_t) + len + 1);
  }

  if (!h) {
    return NULL;
  }

  h->len = len;

  if (len && init) {
    memcpy(h->data, init, len);
  }

  h->data[len] = '\0';

  return (char *)h->data;
}

void guava_string_free(const guava_string_t gs) {
  guava_string_header_t *h = (guava_string_header_t *)((char *)gs - sizeof(guava_string_header_t));
  free(h);
}

guava_string_t guava_string_append(const guava_string_t gs, const guava_string_t gs2) {
  guava_string_header_t *h = (guava_string_header_t *)((char *)gs2 - sizeof(guava_string_header_t));
  return guava_string_append_raw_size(gs, h->data, h->len);
}

guava_string_t guava_string_append_raw(const guava_string_t gs, const char *s) {
  return guava_string_append_raw_size(gs, s, s ? strlen(s) : 0);
}

guava_string_t guava_string_append_raw_size(const guava_string_t gs, const char *s, size_t len) {
  guava_string_header_t *h = NULL;
  if (!gs) {
    return guava_string_new_size(s, len);
  }

  h = (guava_string_header_t *)((char *)gs - sizeof(guava_string_header_t));
  h = realloc(h, sizeof(guava_string_header_t) + h->len + len + 1);

  memcpy(h->data + h->len, s, len);
  h->len += len;
  h->data[h->len] = '\0';

  return h->data;
}

guava_string_t guava_string_append_int(const guava_string_t gs, int i) {
  char buf[128] = {0};
  snprintf(buf, sizeof(buf), "%d", i);
  return guava_string_append_raw(gs, buf);
}

guava_bool_t guava_string_starts_with(const guava_string_t s1, const guava_string_t s2) {
  if (strstr(s1, s2)) {
    return GUAVA_TRUE;
  }

  return GUAVA_FALSE;
}

guava_bool_t guava_string_equal_raw(const guava_string_t s, const char *s2) {
  if (strncmp(s, s2, guava_string_len(s)) == 0) {
    return GUAVA_TRUE;
  }

  return GUAVA_FALSE;
}

size_t guava_string_common_string_count_from_start(const guava_string_t s, const guava_string_t s2) {
  ssize_t count = 0;

  for (size_t i = 0, l = guava_string_len(s); i < l; ++i) {
    if (s2[i] && s[i] && s2[i] == s[i]) {
      ++count;
    }
  }

  return count;
}
