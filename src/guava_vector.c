/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_vector.h"
#include "guava_memory.h"
#include "guava_string.h"

guava_vector_t *
guava_vector_new(size_t count)
{
  guava_vector_t *vec = (guava_vector_t *)guava_calloc(1, sizeof(guava_vector_t));
  if (NULL == vec) return NULL;

  vec->count = 0;
  if (count == 0) {
    vec->data = NULL;
  } else {
    if (NULL == (vec->data = (void **)guava_calloc(1, sizeof(char *) * count))) {
      guava_free(vec);
      return NULL;
    }
  }

  vec->capacity = count;
  return vec;
}

guava_bool_t
guava_vector_push(guava_vector_t *vec,
		  void           *data)
{
  if (vec->count >= vec->capacity) {
    guava_vector_expand(vec, (vec->capacity == 0) ? 1 : vec->capacity << 1);
  }
  vec->data[vec->count] = data;
  ++vec->count;
  return GUAVA_TRUE;
}

guava_bool_t
guava_vector_empty(guava_vector_t *vec)
{
  return (vec->count == 0) ? GUAVA_TRUE : GUAVA_FALSE;
}

size_t
guava_vector_count(guava_vector_t *vec)
{
  return vec->count;
}

size_t
guava_vector_capacity(guava_vector_t *vec)
{
  return vec->capacity;
}

guava_bool_t
guava_vector_reserve(guava_vector_t *vec,
		     size_t          size)
{
  return guava_vector_expand(vec, size);
}

guava_bool_t
guava_vector_expand(guava_vector_t *vec,
		    size_t          size)
{
  if (size <= vec->capacity) return GUAVA_TRUE;
  if (vec->data == NULL) {
    vec->data = (void **)guava_calloc(1, size * sizeof(char *));
  } else {
    vec->data = (void **)guava_realloc(vec->data, size * sizeof(char *));
  }
  memset(&(vec->data[vec->capacity]), 0, (size - vec->capacity) * sizeof(char *));
  vec->capacity = size;
  return GUAVA_TRUE;
}

guava_bool_t
guava_vector_destroy(guava_vector_t         *vec,
		     guava_vector_free_func  fn)
{
  if (vec->data != NULL) {
    for (size_t i = 0; i < vec->count; ++i) {
      if (fn) {
	fn(vec->data[i]);
      }
    }
    guava_free(vec->data);
  }

  guava_free(vec);
  return GUAVA_TRUE;
}

void *
guava_vector_index(guava_vector_t *vec,
		   size_t          vidx)
{
  if (vidx >= vec->count) return NULL;
  return vec->data[vidx];
}

void *
guava_vector_update(guava_vector_t *vec,
		    size_t          vidx,
		    void           *new_value)
{
  if (vidx >= vec->count) return NULL;
  void *old_value = vec->data[vidx];
  vec->data[vidx] = new_value;
  return old_value;
}

void
guava_vector_print(guava_vector_t                *vec,
		   guava_vector_value_print_func  fn)
{
  for (ssize_t i = 0; i < vec->count; ++i) {
    if (fn) {
      printf("##########################\n");
      fn(vec->data[i]);
      printf("##########################\n");
    }
  }
}

guava_vector_iter_t *
guava_vector_iter_create(guava_vector_t *vec)
{
  guava_vector_iter_t *it = (guava_vector_iter_t *)guava_calloc(1, sizeof(guava_vector_iter_t));
  if (NULL == it) return NULL;
  it->vec = vec;
  it->idx = 0;
  return it;
}

guava_bool_t
guava_vector_iter_valid(guava_vector_iter_t *it)
{
  return (it->idx >= guava_vector_count(it->vec)) ? GUAVA_FALSE : GUAVA_TRUE;
}

void *
guava_vector_iter_get_value(guava_vector_iter_t *it)
{
  if (it->idx >= guava_vector_count(it->vec)) return NULL;
  return guava_vector_index(it->vec, it->idx);
}

guava_bool_t
guava_vector_iter_next(guava_vector_iter_t *it)
{
  if (it->idx >= guava_vector_count(it->vec)) return GUAVA_FALSE;
  ++it->idx;
  return GUAVA_TRUE;
}

guava_bool_t
guava_vector_iter_destroy(guava_vector_iter_t *it)
{
  guava_free(it);
  return GUAVA_TRUE;
}

#if 0
int
main(int argc, char **argv)
{
  char *v1 = "V1", *v2 = "V2", *v3 = "V3", *v4 = "V4";
  guava_vector_t *vec = guava_vector_new(0);
  guava_vector_push(vec, v1);
  guava_vector_push(vec, v2);
  guava_vector_push(vec, v3);
  guava_vector_push(vec, v4);
  assert(guava_vector_count(vec) == 4);
  assert(guava_vector_capacity(vec) == 4);
  char *p1 = guava_vector_index(vec, 0);
  char *p2 = guava_vector_index(vec, 1);
  char *p3 = guava_vector_index(vec, 2);
  char *p4 = guava_vector_index(vec, 3);
  assert(memcmp(p1, v1, strlen(p1)) == 0);
  assert(memcmp(p2, v2, strlen(p2)) == 0);
  assert(memcmp(p3, v3, strlen(p3)) == 0);
  assert(memcmp(p4, v4, strlen(p4)) == 0);
  guava_vector_destroy(vec, NULL);
}
#endif
