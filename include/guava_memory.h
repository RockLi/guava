/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_MEMORY_H__
#define __GUAVA_MEMORY_H__

#include "guava.h"

#ifdef GUAVA_MEM_DEBUG

void *
guava_malloc(size_t size);

void *
guava_calloc(size_t count,
             size_t size);

void
guava_free(void *p);

void *
guava_realloc(void   *p,
              size_t  size);

size_t
guava_malloc_size(void *p);

#else

#define guava_malloc(size) malloc((size))
#define guava_calloc(count, size) calloc((count), (size))
#define guava_free(p) free((p))
#define guava_realloc(p, size) realloc((p), (size))

#endif

#endif /* !__GUAVA_MEMORY_H__ */
