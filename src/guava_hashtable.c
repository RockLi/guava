/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_hashtable.h"
#include "guava_string.h"
#include "guava_memory.h"
#include <assert.h>

uint32_t
guava_hashtable_key_hash_func_default(const char *key)
{
  char     *ptr  = (char *)key;
  uint32_t  hash = 0;
  uint32_t  x	 = 0;
  while (*ptr) {
    hash = (hash << 4) + (*ptr++);
    if ((x = hash & 0xF0000000L) != 0)
      {
	hash ^= (x >> 24);
	hash &= ~x;
      }
  }
  return (hash & 0x7FFFFFFF);
}

guava_bool_t
guava_hashtable_key_cmp_func_default(const char *key1,
                                     const char *key2)
{
  if ((strlen(key1) == strlen(key2)) && strcmp(key1, key2) == 0) {
    return GUAVA_TRUE;
  }

  return GUAVA_FALSE;
}

guava_hashtable_t *
guava_hashtable_new(uint32_t                         hint_size,
                    guava_hashtable_key_hash_func    key_hash_fn,
                    guava_hashtable_key_cmp_func     key_cmp_fn,
                    guava_hashtable_key_free_func    key_free_fn,
                    guava_hashtable_value_free_func  value_free_fn,
                    guava_hashtable_value_print_func value_print_fn)
{
  guava_hashtable_t *ht = (guava_hashtable_t *)guava_calloc(1, sizeof(guava_hashtable_t));
  if (!ht) {
    return NULL;
  }

  ht->key_hash_fn = key_hash_fn ? key_hash_fn : guava_hashtable_key_hash_func_default;
  ht->key_cmp_fn = key_cmp_fn ? key_cmp_fn : guava_hashtable_key_cmp_func_default;
  ht->key_free_fn = key_free_fn ? key_free_fn : NULL;
  ht->value_free_fn = value_free_fn ? value_free_fn : NULL;
  ht->value_print_fn = value_print_fn ? value_print_fn : NULL;
  ht->buckets = (guava_hashtable_entry_t **)guava_calloc(1, sizeof(guava_hashtable_entry_t *) * hint_size);
  ht->bucket_count = hint_size;
  ht->entry_count = 0;
  return ht;
}

guava_bool_t
guava_hashtable_reserve(guava_hashtable_t *h,
			uint32_t           bucket_count)
{
  if (bucket_count <= h->bucket_count) {
    return GUAVA_TRUE;
  }

  h->buckets = (guava_hashtable_entry_t **)guava_realloc(h->buckets, bucket_count * sizeof(guava_hashtable_entry_t *));
  memset(&(h->buckets[h->bucket_count]), 0, (bucket_count - h->bucket_count) * sizeof(guava_hashtable_entry_t *));
  h->bucket_count = bucket_count;
  return GUAVA_TRUE;
}

guava_bool_t
guava_hashtable_insert(guava_hashtable_t *h,
                       const char        *key,
                       void              *value)
{
  if ((double)++h->entry_count / (double)h->bucket_count >= 1.20) {
    guava_hashtable_expand(h, h->entry_count << 1); // to expand to 2 * size
  }

  guava_hashtable_entry_t *entry = (guava_hashtable_entry_t *)guava_calloc(1, sizeof(guava_hashtable_entry_t));
  if (!entry) {
    return GUAVA_FALSE;
  }

  entry->hash = h->key_hash_fn(key);
  uint32_t index = entry->hash % h->bucket_count;
  entry->next = h->buckets[index];
  entry->key = key;
  entry->value = value;
  h->buckets[index] = entry;

  return GUAVA_TRUE;
}

void *
guava_hashtable_delete(guava_hashtable_t *h,
		       const char        *key)
{
  uint32_t hash = h->key_hash_fn(key);
  guava_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  void *value = NULL;
  while (entry) {
    if ((hash == entry->hash) && (h->key_cmp_fn(entry->key, key))) {
      --h->entry_count;
      h->buckets[hash % h->bucket_count] = entry->next;
      value = entry->value;
      if (h->key_free_fn) {
        h->key_free_fn((void *)(entry->key));
      }
      guava_free(entry);
      return value;
    }
    entry = entry->next;
  }

  return NULL;
}

guava_bool_t
guava_hashtable_update(guava_hashtable_t *h,
		       const char        *key,
		       void              *value)
{
  uint32_t hash = h->key_hash_fn(key);
  guava_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  while (entry) {
    if ((hash == entry->hash) && (h->key_cmp_fn(entry->key, key))) {
      if (h->value_free_fn) {
        h->value_free_fn(entry->value);
      }
      entry->value = value;
      return GUAVA_TRUE;
    }
    entry = entry->next;
  }

  return GUAVA_FALSE;
}

void *
guava_hashtable_find(guava_hashtable_t *h,
		     const char        *key)
{
  uint32_t hash = h->key_hash_fn(key);
  guava_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  while (entry) {
    if ((hash == entry->hash) && (h->key_cmp_fn(entry->key, key))) {
      return entry->value;
    }
    entry = entry->next;
  }

  return NULL;
}

uint32_t
guava_hashtable_count(guava_hashtable_t *h)
{
  return h->entry_count;
}

uint32_t
guava_hashtable_bucket_count(guava_hashtable_t *h)
{
  return h->bucket_count;
}

void
guava_hashtable_destroy(guava_hashtable_t *h)
{
  guava_hashtable_entry_t *entry = NULL;
  uint32_t i = 0;
  for (; i < h->bucket_count; ++i) {
    entry = h->buckets[i];
    while (entry) {
      h->buckets[i] = entry->next;
      if (h->key_free_fn) {
        h->key_free_fn((void *)(entry->key));
      }
      if (h->value_free_fn) {
	h->value_free_fn(entry->value);
      }
      guava_free(entry);
      --h->entry_count;
      entry = h->buckets[i];
    }
  }
  guava_free(h->buckets);
  guava_free(h);
}

guava_bool_t
guava_hashtable_expand(guava_hashtable_t *h,
		       uint32_t           new_size)
{
  if (new_size <= h->bucket_count) {
    return GUAVA_TRUE;
  }

  guava_hashtable_entry_t **new_buckets = (guava_hashtable_entry_t **)guava_calloc(1, sizeof(guava_hashtable_entry_t *) * new_size);
  if (!new_buckets) {
    return GUAVA_FALSE;
  }
  if (!new_buckets) {
    return GUAVA_FALSE;
  }
  guava_hashtable_entry_t *entry = NULL;
  uint32_t i = 0, index;
  for (; i < h->bucket_count; ++i) {
    while ((entry = h->buckets[i]) != NULL) {
      h->buckets[i] = entry->next;
      index = entry->hash % new_size;
      entry->next = new_buckets[index];
      new_buckets[index] = entry;
    }
  }
  guava_free(h->buckets);
  h->buckets = new_buckets;
  h->bucket_count = new_size;
  return GUAVA_TRUE;
}

guava_bool_t
guava_hashtable_compact(guava_hashtable_t *h,
			int32_t            flags)
{
  // @FixMe
  return GUAVA_TRUE;
}

void
guava_hashtable_print(guava_hashtable_t *h)
{
  guava_hashtable_iter_t *it = guava_hashtable_iter_new(h);
  printf("=========BEGIN DUMP HASHTABLE ==================\n");
  while (guava_hashtable_iter_valid(it)) {
    void *value = guava_hashtable_iter_get_value(it);
    printf("Key: %s\n", guava_hashtable_iter_get_key(it));
    printf("Value: ");
    if (h->value_print_fn) {
      h->value_print_fn(value);
    }
    printf("\n");
    if (guava_hashtable_iter_next(it) == GUAVA_FALSE) break;
  }
  guava_hashtable_iter_destroy(it);
  printf("=================  END =========================\n");
}

guava_hashtable_iter_t *
guava_hashtable_iter_new(guava_hashtable_t *h)
{
  guava_hashtable_iter_t *it = (guava_hashtable_iter_t *)guava_calloc(1, sizeof(guava_hashtable_iter_t));
  if (!it) {
    return NULL;
  }
  it->h = h;
  it->entry = NULL;
  it->entry_parent = NULL;
  it->idx = h->bucket_count;
  if (0 == h->entry_count) {
    return it;
  }
  uint32_t i = 0;
  for (; i < it->idx; ++i) {
    if (h->buckets[i] != NULL) {
      it->idx = i;
      it->entry = h->buckets[i];
      break;
    }
  }
  return it;
}

guava_bool_t
guava_hashtable_iter_valid(guava_hashtable_iter_t *it)
{
  return (it->entry == NULL && it->idx == it->h->bucket_count) ? GUAVA_FALSE : GUAVA_TRUE;
}

const char *
guava_hashtable_iter_get_key(guava_hashtable_iter_t *it)
{
  return it->entry->key;
}

void *
guava_hashtable_iter_get_value(guava_hashtable_iter_t *it)
{
  return it->entry->value;
}

guava_hashtable_entry_t *
guava_hashtable_iter_get_entry(guava_hashtable_iter_t *it)
{
  return it->entry;
}

guava_bool_t
guava_hashtable_iter_next(guava_hashtable_iter_t *it)
{
  if (NULL == it->entry && it->h->bucket_count == it->idx) {
    return GUAVA_FALSE;
  }
  if (it->entry->next != NULL) {
    it->entry_parent = it->entry;
    it->entry = it->entry->next;
    return GUAVA_TRUE;
  }
  it->entry_parent = NULL;
  it->entry = NULL;
  if (it->h->bucket_count <= ++it->idx) {
    return GUAVA_FALSE;
  }
  uint32_t i = it->idx;
  for (; i < it->h->bucket_count; ++i) {
    if (it->h->buckets[i] != NULL) {
      it->idx = i;
      it->entry = it->h->buckets[i];
      return GUAVA_TRUE;
    }
  }
  it->idx = it->h->bucket_count;
  return GUAVA_FALSE;
}

guava_bool_t
guava_hashtable_iter_destroy(guava_hashtable_iter_t *it)
{
  guava_free(it);
  return GUAVA_TRUE;
}

#if 0
void print_value(void *value){
  printf("%s", (const char *)value);
}

int
main(int argc, char **argv)
{
  guava_hashtable_t *h = guava_hashtable_new(16,
                                             guava_hashtable_key_hash_func_default,
                                             guava_hashtable_key_cmp_func_default,
                                             NULL,
                                             NULL,
                                             print_value);
  // T1
  assert(guava_hashtable_count(h) == 0);
  assert(guava_hashtable_bucket_count(h) == 16);
  guava_hashtable_print(h);

  // T2
  guava_hashtable_reserve(h, 100);
  assert(guava_hashtable_count(h) == 0);
  assert(guava_hashtable_bucket_count(h) == 100);
  guava_hashtable_print(h);

  // T3
  guava_hashtable_insert(h, "name", "rock");
  assert(guava_hashtable_count(h) == 1);
  guava_hashtable_print(h);

  guava_hashtable_insert(h, "country", "China");
  assert(guava_hashtable_count(h) == 2);
  guava_hashtable_print(h);

  guava_hashtable_iter_t *it = guava_hashtable_iter_new(h);
  while (guava_hashtable_iter_valid(it)) {
    const char *key = guava_hashtable_iter_get_key(it);
    void *value = guava_hashtable_iter_get_value(it);

    if (strncmp(key, "name", strlen(key)) == 0) {
      assert(strncmp(value, "rock", strlen(value)) == 0);
    } else if (strncmp(key, "country", strlen(key)) == 0) {
      assert(strncmp(value, "China", strlen(value)) == 0);
    } else {
      assert(1 == 0);
    }

    if (guava_hashtable_iter_next(it) == GUAVA_FALSE)
      break;
  }
  guava_hashtable_iter_destroy(it);

  // T4
  void *value = guava_hashtable_delete(h, "name");
  assert(guava_hashtable_count(h) == 1);

  guava_hashtable_print(h);

  guava_hashtable_destroy(h);
}
#endif
