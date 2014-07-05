/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_memory.h"

#ifdef GUAVA_MEM_DEBUG
#include <assert.h>

const uint32_t guava_mem_magic_number = 0x526F636B;

void *guava_malloc(size_t size)
{
  void *ptr = calloc(1, size + 8 + sizeof(size_t));
  if (NULL == ptr) return NULL;
  memcpy(ptr, &guava_mem_magic_number, 4);
  memcpy(ptr + 4, &size, sizeof(size));
  memcpy(ptr + size + 8 + sizeof(size_t) - 4, &guava_mem_magic_number, 4);
  return ptr + 4 + sizeof(size_t);
}

void *
guava_calloc(size_t count,
	     size_t size)
{
  return guava_malloc(size * count);
}

void
guava_free(void *ptr)
{
  void *p = ptr - 4 - sizeof(size_t);
  assert(*((uint32_t *)p) == guava_mem_magic_number);
  size_t size = *((size_t *)(p + 4));
  assert(*((uint32_t *)(p + 4 + sizeof(size_t) + size)) == guava_mem_magic_number);
  free(p);
  p = NULL;
}

void *
guava_realloc(void *ptr,
	      size_t size)
{
  void *p = realloc(ptr - 4 - sizeof(size_t), 4 + sizeof(size_t) + size + 4);
  memcpy(p + 4, &size, sizeof(size));
  memcpy(p + 4 + sizeof(size_t) + size, &guava_mem_magic_number, 4);
  return p + 4 + sizeof(size_t);
}


size_t
guava_malloc_size(void *ptr)
{
  return (*(size_t *)(ptr - sizeof(size_t)));
}

#endif
