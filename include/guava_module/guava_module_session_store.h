/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_MODULE_SESSION_STORE_H__
#define __GUAVA_MODULE_SESSION_STORE_H__

#include "guava_module/guava_module.h"
#include "guava_session/guava_session_store.h"

typedef struct {
  PyObject_HEAD
  guava_session_store_t *store;
} Py_SessionStore;

typedef struct {
  Py_SessionStore  py_store;
  PyObject        *data;
} Py_MemSessionStore;

typedef struct {
  Py_SessionStore  py_store;
  guava_string_t   directory;
} Py_FileSessionStore;

extern PyTypeObject PyType_SessionStore;

extern PyTypeObject PyType_MemSessionStore;

extern PyTypeObject PyType_FileSessionStore;

#endif /* !__GUAVA_MODULE_SESSION_STORE_H__ */
