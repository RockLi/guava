/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_VECTOR_H__
#define __GUAVA_VECTOR_H__

#include "guava.h"

typedef struct guava_vector {
  size_t    count;
  size_t    capacity;
  void    **data;
} guava_vector_t;

typedef struct guava_vector_iter {
  guava_vector_t *vec;
  size_t          idx;
} guava_vector_iter_t;

typedef void (*guava_vector_free_func)(void *);
typedef void (*guava_vector_value_print_func)(void *);

/**
 * Create a new vector
 */
guava_vector_t *
guava_vector_new(size_t count);

/**
 * Check whether the vector is empty or not
 */
guava_bool_t
guava_vector_empty(guava_vector_t *vec);

/**
 * Count the vector
 */
size_t
guava_vector_count(guava_vector_t *vec);

/**
 * Reserve the vector
 */
guava_bool_t
guava_vector_reserve(guava_vector_t *vec,
		     size_t          size);

/**
 * Expand the vector
 */
guava_bool_t
guava_vector_expand(guava_vector_t *vec,
		    size_t          size);

/**
 * Push an element to the vector
 */
guava_bool_t
guava_vector_push(guava_vector_t *vec,
                  void           *data);

/**
 * Destroy the vector
 */
guava_bool_t
guava_vector_destroy(guava_vector_t         *vec,
		     guava_vector_free_func  fn);

/**
 * Get the element with specific index
 */
void *
guava_vector_index(guava_vector_t *vec,
		   size_t          vidx);

/**
 * Replace the value in specific position
 */
void *
guava_vector_update(guava_vector_t *vec,
		    size_t          vidx,
		    void           *new_value);

/**
 * Print the elements in the vector
 */
void
guava_vector_print(guava_vector_t                *vec,
		   guava_vector_value_print_func  fn);

/**
 * Create a new iterater for iterating a vector
 */
guava_vector_iter_t *
guava_vector_iter_create(guava_vector_t *vec);

/**
 * Check whether the iterator is valid or not
 */
guava_bool_t
guava_vector_iter_valid(guava_vector_iter_t *it);

/**
 * Get the value
 */
void *
guava_vector_iter_get_value(guava_vector_iter_t *it);

/**
 * Advance the iterator
 */
guava_bool_t
guava_vector_iter_next(guava_vector_iter_t *it);

/**
 * Destroy the iterator
 */
guava_bool_t
guava_vector_iter_destroy(guava_vector_iter_t *it);

#endif /* !__GUAVA_VECTOR_H__ */
