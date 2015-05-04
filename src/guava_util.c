/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_util.h"

void
guava_lowercase(char *s, size_t len)
{
  for (size_t i = 0; i < len; ++i) {
    s[i] = tolower(s[i]);
  }
}
