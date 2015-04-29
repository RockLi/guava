/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_mime_type.h"

typedef struct {
  const char *ext;
  const char *mime_type;
} guava_mime_type_t;

static guava_mime_type_t mime_types[] = {
  {"html", "text/html"},
  {"js", "text/javascript"},
  {"css", "text/css"},
  {"txt", "text/plain"},
  {"png", "image/png"},
  {"gif", "image/gif"},
  {"jpg", "image/jpeg"},
  {"jpeg", "image/jpeg"},
  {"ico", "image/x-icon"},
};

const char *guava_mime_type_guess(const char *filename) {
  char *pos = strrchr(filename, '.');
  if (!pos) {
    return "text/plain";
  }

  for (int i = 0; i < sizeof(mime_types) / sizeof(mime_types[0]); ++i) {
    if (strcmp(mime_types[i].ext, pos+1) == 0) {
      return mime_types[i].mime_type;
    }
  }

  return "text/plain";
}
