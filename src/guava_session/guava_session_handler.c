/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_session/guava_session_handler.h"
#include "guava_string.h"
#include "guava_hashtable.h"
#include "guava_memory.h"
#include "guava_util.h"

guava_string_t guava_session_handler_create_session_id_default()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);

  char buf[64] = {0};
  snprintf(buf, sizeof(buf), "%.15d%ld", tv.tv_sec, tv.tv_usec);

  return guava_string_new_size(buf, 16);
}
