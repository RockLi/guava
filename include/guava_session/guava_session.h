/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_SESSION_H__
#define __GUAVA_SESSION_H__

#include "guava.h"

#define GUAVA_SESSION_DEFAULT_NAME "sessionid"
#define GUAVA_SESSION_DEFAULT_GC_TIME (7 * 86400)
#define GUAVA_SESSION_DEFAULT_COOKIE_TIME 0
#define GUAVA_SESSION_DEFAULT_COOKIE_PATH "/"
#define GUAVA_SESSION_DEFAULT_COOKIE_DOMAIN NULL
#define GUAVA_SESSION_DEFAULT_COOKIE_SECURE GUAVA_FALSE
#define GUAVA_SESSION_DEFAULT_COOKIE_HTTPONLY GUAVA_FALSE

typedef struct {
  guava_session_store_t  store;
  PyObject              *items; /* Dict for storing key-value data  */
} guava_session_store_inmem_t;

typedef struct {
  guava_session_store_t store;
  guava_string_t        path;
} guava_session_store_file_t;

typedef guava_string_t guava_session_id_t;

guava_session_id_t guava_session_new_id(void);

guava_session_id_t guava_session_new_id_raw(const char *id);

void guava_session_id_free(guava_session_id_t id);

guava_session_store_t *guava_session_store_new(guava_session_store_type_t type);

void guava_session_store_free(guava_session_store_t *store);

PyObject *guava_session_get(guava_session_store_t *store, guava_session_id_t id);

PyObject *guava_session_set(guava_session_store_t *store, guava_session_id_t id, PyObject *items);

PyObject *guava_session_delete(guava_session_store_t *store, guava_session_id_t id);

#endif /* !__GUAVA_SESSION_H__ */
