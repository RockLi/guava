/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_string.h"
#include "guava_session/guava_session.h"
#include "guava_session/guava_session_store_inmem.h"
#include "guava_session/guava_session_store_file.h"
#include "guava_memory.h"

#define GUAVA_SESSION_MAGIC "avaug"
#define GUAVA_SESSION_MAGIC_END "guava"

guava_session_id_t guava_session_new_id(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  char data[256] = {0};
  snprintf(data, sizeof(data), "%s%d%d%d%d%s", GUAVA_SESSION_MAGIC, t.tv_sec, t.tv_usec, getpid(), rand_r((int *)&t.tv_usec), GUAVA_SESSION_MAGIC_END);
  return guava_string_new(data);
}

guava_session_id_t guava_session_new_id_raw(const char *id) {
  return guava_string_new(id);
}

void guava_session_id_free(guava_session_id_t id) {
  if (id) {
    guava_string_free(id);
  }
}

guava_session_store_t *guava_session_store_new(guava_session_store_type_t type) {
  guava_session_new_id();

  guava_session_store_t *store = NULL;

  switch(type) {
  case GUAVA_SESSION_STORE_INMEM: {
    store = (guava_session_store_t *)guava_session_store_inmem_new();
    break;
  }

  case GUAVA_SESSION_STORE_FILE: {
    store = (guava_session_store_t *)guava_session_store_file_new();
    break;
  }

  default:
    fprintf(stderr, "Unknown session store type\n");
    break;
  }

  if (!store) {
    return NULL;
  }

  store->name = GUAVA_SESSION_DEFAULT_NAME;
  store->gc_time = GUAVA_SESSION_DEFAULT_GC_TIME;
  store->cookie_time = GUAVA_SESSION_DEFAULT_COOKIE_TIME;
  store->cookie_path = GUAVA_SESSION_DEFAULT_COOKIE_PATH;
  store->cookie_domain = GUAVA_SESSION_DEFAULT_COOKIE_DOMAIN;
  store->cookie_secure = GUAVA_SESSION_DEFAULT_COOKIE_SECURE;
  store->cookie_httponly = GUAVA_SESSION_DEFAULT_COOKIE_HTTPONLY;
  store->type = type;

  return store;
}

void guava_session_store_free(guava_session_store_t *store) {
  if (!store) {
    return;
  }

  switch(store->type) {

  case GUAVA_SESSION_STORE_INMEM: {
    guava_session_store_inmem_free((guava_session_store_inmem_t *)store);
    break;
  }

  case GUAVA_SESSION_STORE_FILE: {
    guava_session_store_file_free((guava_session_store_file_t *)store);
    break;
  }

  default:
    fprintf(stderr, "unknown store type\n");
    break;
  }
}

PyObject *guava_session_get(guava_session_store_t *store, guava_session_id_t id) {
  if (!store || !id) {
    Py_RETURN_NONE;
  }

  switch(store->type) {

  case GUAVA_SESSION_STORE_INMEM: {
    return guava_session_store_inmem_get((guava_session_store_inmem_t *)store, id);
  }

  case GUAVA_SESSION_STORE_FILE: {
    return guava_session_store_file_get((guava_session_store_file_t *)store, id);
  }

  default:
    fprintf(stderr, "Unknown store type\n");
    break;
  }

  Py_RETURN_NONE;
}

PyObject *guava_session_set(guava_session_store_t *store, guava_session_id_t id, PyObject *items) {
  if (!store || !id || !items) {
    Py_RETURN_FALSE;
  }

  switch(store->type) {

  case GUAVA_SESSION_STORE_INMEM:
    return guava_session_store_inmem_set((guava_session_store_inmem_t *)store, id, items);

  case GUAVA_SESSION_STORE_FILE:
    return guava_session_store_file_set((guava_session_store_file_t *)store, id, items);

  default:
    fprintf(stderr, "Unknown store type\n");
    break;
  }

  Py_RETURN_FALSE;
}

PyObject *guava_session_delete(guava_session_store_t *store, guava_session_id_t id) {
  if (!store || !id) {
    Py_RETURN_FALSE;
  }

  PyObject *v = Py_False;

  switch(store->type) {
  case GUAVA_SESSION_STORE_INMEM: {
    v = guava_session_store_inmem_delete((guava_session_store_inmem_t *)store, id);
    break;
  }

  case GUAVA_SESSION_STORE_FILE: {
    v = guava_session_store_file_delete((guava_session_store_file_t *)store, id);
    break;
  }

  default:
    fprintf(stderr, "Unknown store type\n");
    break;
  }

  Py_INCREF(v);
  return v;
}
