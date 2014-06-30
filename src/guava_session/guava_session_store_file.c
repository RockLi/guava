/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_session/guava_session_store_file.h"
#include "guava_string.h"
#include <marshal.h>

void guava_session_store_file_get_file_name(guava_session_store_file_t *store, guava_session_id_t id, char *path, size_t path_len) {
  snprintf(path, path_len, "%s/%s", store->path, id);
}

FILE *guava_session_store_file_open(guava_session_store_file_t *store, guava_session_id_t id, const char *mode) {
  char path[MAXPATH] = {0};
  guava_session_store_file_get_file_name(store, id, path, sizeof(path));
  FILE *fp = fopen(path, mode);
  return fp;
}

guava_session_store_file_t *guava_session_store_file_new(void) {
  guava_session_store_file_t *store = (guava_session_store_file_t *)calloc(1, sizeof(*store));
  if (!store) {
    return NULL;
  }

  store->path = guava_string_new("/tmp");
  return store;
}

void guava_session_store_file_free(guava_session_store_file_t *store) {
  if (!store) {
    return;
  }

  if (store->path) {
    guava_string_free(store->path);
  }

  free(store);
}

PyObject *guava_session_store_file_get(guava_session_store_file_t *store, guava_session_id_t id) {
  FILE *fp = guava_session_store_file_open(store, id, "rb");
  if (!fp) {
    Py_RETURN_NONE;
  }

  PyObject *obj = PyMarshal_ReadObjectFromFile(fp);
  fclose(fp);

  if (!obj) {
    Py_RETURN_NONE;
  }

  Py_INCREF(obj);

  return obj;
}

PyObject *guava_session_store_file_set(guava_session_store_file_t *store, guava_session_id_t id, PyObject *value) {
  if (!PyDict_Check(value)) {
    Py_RETURN_FALSE;
  }

  FILE *fp = guava_session_store_file_open(store, id, "wb");
  if (!fp) {
    Py_RETURN_FALSE;
  }

  PyMarshal_WriteObjectToFile(value, fp, Py_MARSHAL_VERSION);

  fclose(fp);

  Py_RETURN_TRUE;
}

PyObject *guava_session_store_file_delete(guava_session_store_file_t *store, guava_session_id_t id) {
  char path[MAXPATH] = {0};
  guava_session_store_file_get_file_name(store, id, path, sizeof(path));

  if (unlink(path) < 0) {
    Py_RETURN_FALSE;
  }

  Py_RETURN_TRUE;
}
