/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_HASHTABLE_H__
#define __GUAVA_HASHTABLE_H__

#include "guava.h"

typedef struct guava_hashtable_entry {
  struct guava_hashtable_entry *next;
  struct guava_hashtable_entry *prev;
  const char                   *key;
  void                         *value;
  uint32_t                      hash;
} guava_hashtable_entry_t;

typedef uint32_t (*guava_hashtable_key_hash_func)(const char *key);
typedef guava_bool_t (*guava_hashtable_key_cmp_func)(const char *key1, const char *key2);
typedef void (*guava_hashtable_key_free_func)(void *);
typedef void (*guava_hashtable_value_free_func)(void *);
typedef void (*guava_hashtable_value_print_func)(void *);

typedef struct guava_hashtable {
  guava_hashtable_entry_t          **buckets;
  uint32_t                           bucket_count;
  uint32_t                           entry_count;
  guava_hashtable_key_hash_func      key_hash_fn;
  guava_hashtable_key_cmp_func       key_cmp_fn;
  guava_hashtable_key_free_func      key_free_fn;
  guava_hashtable_value_free_func    value_free_fn;
  guava_hashtable_value_print_func   value_print_fn;
} guava_hashtable_t;

typedef struct guava_hashtable_iter {
  guava_hashtable_t       *h;
  guava_hashtable_entry_t *entry;
  guava_hashtable_entry_t *entry_parent;
  uint32_t                 idx;
} guava_hashtable_iter_t;

/**
 * Create a new Hashtable
 */
guava_hashtable_t *
guava_hashtable_new(uint32_t                         hint_size,
                    guava_hashtable_key_hash_func    key_hash_fn,
                    guava_hashtable_key_cmp_func     key_cmp_fn,
                    guava_hashtable_key_free_func    key_free_fn,
                    guava_hashtable_value_free_func  value_free_fn,
                    guava_hashtable_value_print_func value_print_fn);

/**
 * Insert one entry into the hashtable
 */
guava_bool_t
guava_hashtable_insert(guava_hashtable_t *h,
                       const char        *key,
                       void              *value);

/**
 * Update one entry in the hashtable
 */
guava_bool_t
guava_hashtable_update(guava_hashtable_t *h,
                       const char        *key,
                       void              *data);


/**
 * Find/Lookup one entry in the hashtable
 */
void *
guava_hashtable_find(guava_hashtable_t *h,
                     const char        *key);

/**
 * Delete one entry from the hashtable
 */
void *
guava_hashtable_delete(guava_hashtable_t *h,
                       const char        *key);

/**
 * Delete all entries from the hashtable
 */
void
guava_hashtable_clear(guava_hashtable_t *h);

/**
 * Expand the hashtable
 */
guava_bool_t
guava_hashtable_expand(guava_hashtable_t *h,
		       uint32_t           new_size);

/**
 * Clear all entries and release all resources
 *
 * Similiar to the clear api, but will release all internal resources
 */
void guava_hashtable_destroy(guava_hashtable_t *h);

/**
 * Create a new iterator to iterate entries in the hashtable
 */
guava_hashtable_iter_t *
guava_hashtable_iter_new(guava_hashtable_t *h);

/**
 * Check whether the iterator is valid or not
 */
guava_bool_t
guava_hashtable_iter_valid(guava_hashtable_iter_t *it);

/**
 * Get the key of the entry in the hashtable
 */
const char *
guava_hashtable_iter_get_key(guava_hashtable_iter_t *it);

/**
 * Get the value of the entry in the hashtable
 */
void *
guava_hashtable_iter_get_value(guava_hashtable_iter_t *it);

/**
 * Get the entry that the iterator pointed to
 */
guava_hashtable_entry_t *
guava_hashtable_iter_get_entry(guava_hashtable_iter_t *it);

/**
 * Advance the iterator
 */
guava_bool_t
guava_hashtable_iter_next(guava_hashtable_iter_t *it);

/**
 * Destroy the iterator
 */
guava_bool_t
guava_hashtable_iter_destroy(guava_hashtable_iter_t *it);

uint32_t
guava_hashtable_key_hash_func_default(const char *key);

guava_bool_t
guava_hashtable_key_cmp_func_default(const char *key1,
                                     const char *key2);

#endif /* !__GUAVA_HASHTABLE_H__ */
