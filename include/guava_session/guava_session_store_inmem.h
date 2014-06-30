/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_SESSION_STORE_INMEM_H__
#define __GUAVA_SESSION_STORE_INMEM_H__

#include "guava.h"
#include "guava_session/guava_session.h"

guava_session_store_inmem_t *guava_session_store_inmem_new(void);

void guava_session_store_inmem_free(guava_session_store_inmem_t *store);

PyObject *guava_session_store_inmem_get(guava_session_store_inmem_t *store, guava_session_id_t id);

PyObject *guava_session_store_inmem_set(guava_session_store_inmem_t *store, guava_session_id_t id, PyObject *items);

PyObject *guava_session_store_inmem_delete(guava_session_store_inmem_t *store, guava_session_id_t id);

#endif /* !__GUAVA_SESSION_STORE_INMEM_H__ */
