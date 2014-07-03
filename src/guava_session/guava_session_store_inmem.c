/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_session/guava_session_store_inmem.h"
#include "guava_memory.h"

guava_session_store_inmem_t *guava_session_store_inmem_new(void) {
  guava_session_store_inmem_t *store = (guava_session_store_inmem_t *)guava_calloc(1, sizeof(*store));
  if (!store) {
    return NULL;
  }

  store->items = PyDict_New();
  return store;
}

void guava_session_store_inmem_free(guava_session_store_inmem_t *store) {
  if (!store) {
    return;
  }
  Py_DECREF(store->items);
  guava_free(store);
}

PyObject *guava_session_store_inmem_get(guava_session_store_inmem_t *store, guava_session_id_t id) {
  PyObject *obj = PyDict_GetItemString(store->items, id);
  if (!obj) {
    Py_RETURN_NONE;
  }

  Py_INCREF(obj);

  return obj;
}

PyObject *guava_session_store_inmem_set(guava_session_store_inmem_t *store, guava_session_id_t id, PyObject *items) {
  if (!PyDict_Check(items)) {
    Py_RETURN_FALSE;
  }

  if (PyDict_SetItemString(store->items, id, items) == 0) {
    Py_RETURN_TRUE;
  }

  PyErr_Print();

  Py_RETURN_FALSE;
}

PyObject *guava_session_store_inmem_delete(guava_session_store_inmem_t *store, guava_session_id_t id) {
  if (PyDict_DelItemString(store->items, id) == 0) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}
