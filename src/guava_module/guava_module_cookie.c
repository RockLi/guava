/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_cookie.h"
#include "guava_string.h"
#include "guava_module.h"

PyObject *Cookie_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  Cookie *self;

  self = (Cookie *)type->tp_alloc(type, 0);
  if (self) {
  }

  return (PyObject *)self;
}

static void Cookie_dealloc(Cookie *self) {
  self->ob_type->tp_free((PyObject *)self);
}

static int Cookie_init(Cookie *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"name", "value", "path", "domain", "secure", "httponly", "expired", "max_age", NULL};

  char *name = NULL;
  char *value = NULL;
  char *path = NULL;
  char *domain = NULL;
  int secure = GUAVA_FALSE;
  int httponly = GUAVA_FALSE;
  int expired = -1;
  int max_age = -1;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "ss|ssBBi",
                                   kwlist,
                                   &name,
                                   &value,
                                   &path,
                                   &domain,
                                   &secure,
                                   &httponly,
                                   &expired,
                                   &max_age)) {
    PyErr_SetString(PyExc_TypeError, "please give correct parameters");
    return -1;
  }

  if (name) {
    guava_cookie_set_name(&self->data, name);
  }
  if (value) {
    guava_cookie_set_value(&self->data, value);
  }
  if (path) {
    guava_cookie_set_path(&self->data, path);
  }
  if (domain) {
    guava_cookie_set_domain(&self->data, domain);
  }

  guava_cookie_set_secure(&self->data, secure);
  guava_cookie_set_httponly(&self->data, httponly);
  guava_cookie_set_expired(&self->data, expired);
  guava_cookie_set_max_age(&self->data, max_age);

  return 0;
}

static PyObject *Cookie_repr(Cookie *self) {
  char expired[128] = {0};
  if (self->data.expired >= 0) {
    snprintf(expired, sizeof(expired), "%d", self->data.expired);
  }

  char max_age[128] = {0};
  if (self->data.max_age >= 0) {
    snprintf(max_age, sizeof(max_age), "%d", self->data.max_age);
  }

  return PyString_FromFormat("Cookie name(%s), value(%s), domain(%s), path(%s), expired(%s), max-age(%s), secure(%s), httponly(%s)",
                             self->data.name ? self->data.name : "Not Set",
                             self->data.value ? self->data.value : "Not Set",
                             self->data.domain ? self->data.domain : "Not Set",
                             self->data.path ? self->data.path : "Not Set",
                             self->data.expired >= 0 ? expired : "Not Set",
                             self->data.max_age >= 0 ? max_age : "Not Set",
                             self->data.secure ? "True" : "False",
                             self->data.httponly ? "True" : "False");
}

static PyObject *Cookie_get_name(Cookie *self, void *closure) {
  if (self->data.name) {
    return PyString_FromString(self->data.name);
  }

  Py_RETURN_NONE;
}

static int Cookie_set_name(Cookie *self, PyObject *value, void *closure) {
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the name attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The name attribute value must be a string");
    return -1;
  }

  guava_cookie_set_name(&self->data, PyString_AsString(value));

  return 0;
}

static PyObject *Cookie_get_value(Cookie *self, void *closure) {
  if (self->data.value) {
    return PyString_FromString(self->data.value);
  }

  Py_RETURN_NONE;
}

static int Cookie_set_value(Cookie *self, PyObject *value, void *closure) {
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the value attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The value attribute value must be a string");
    return -1;
  }

  guava_cookie_set_value(&self->data, PyString_AsString(value));

  return 0;
}

static PyObject *Cookie_get_domain(Cookie *self, void *closure) {
  if (self->data.domain) {
    return PyString_FromString(self->data.domain);
  }

  Py_RETURN_NONE;
}

static int Cookie_set_domain(Cookie *self, PyObject *value, void *closure) {
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the domain attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The domain attribute value must be a string");
    return -1;
  }

  guava_cookie_set_domain(&self->data, PyString_AsString(value));

  return 0;
}

static PyObject *Cookie_get_path(Cookie *self, void *closure) {
  if (self->data.path) {
    return PyString_FromString(self->data.path);
  }

  Py_RETURN_NONE;
}

static int Cookie_set_path(Cookie *self, PyObject *value, void *closure) {
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the path attribute");
    return -1;
  }

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The path attribute value must be a string");
    return -1;
  }

  guava_cookie_set_path(&self->data, PyString_AsString(value));

  return 0;
}

static PyObject *Cookie_get_expired(Cookie *self, void *closure) {
  return PyInt_FromLong(self->data.expired);
}

static int Cookie_set_expired(Cookie *self, PyObject *value, void *closure) {
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the expired attribute");
    return -1;
  }

  if (!PyInt_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The expired attribute value must be a int");
    return -1;
  }

  guava_cookie_set_expired(&self->data, PyInt_AsLong(value));

  return 0;
}

static PyObject *Cookie_get_max_age(Cookie *self, void *closure) {
  return PyInt_FromLong(self->data.max_age);
}

static int Cookie_set_max_age(Cookie *self, PyObject *value, void *closure) {
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the max_age attribute");
    return -1;
  }

  if (!PyInt_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The max_age attribute value must be a string");
    return -1;
  }

  guava_cookie_set_max_age(&self->data, PyInt_AsLong(value));

  return 0;
}

static PyObject *Cookie_get_secure(Cookie *self, void *closure) {
  return PyBool_FromLong(self->data.secure);
}

static int Cookie_set_secure(Cookie *self, PyObject *value, void *closure) {
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the secure attribute");
    return -1;
  }

  if (!PyBool_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The secure attribute value must be a boolean");
    return -1;
  }

  guava_cookie_set_secure(&self->data, PyInt_AsLong(value));

  return 0;
}

static PyObject *Cookie_get_httponly(Cookie *self, void *closure) {
  return PyBool_FromLong(self->data.httponly);
}

static int Cookie_set_httponly(Cookie *self, PyObject *value, void *closure) {
  if (value == NULL)  {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the httponly attribute");
    return -1;
  }

  if (!PyBool_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The httponly attribute value must be a boolean");
    return -1;
  }

  guava_cookie_set_httponly(&self->data, PyInt_AsLong(value));

  return 0;
}

static PyGetSetDef Cookie_getseter[] = {
  {"name", (getter)Cookie_get_name, (setter)Cookie_set_name, "name", NULL},
  {"value", (getter)Cookie_get_value, (setter)Cookie_set_value, "value", NULL},
  {"domain", (getter)Cookie_get_domain, (setter)Cookie_set_domain, "domain", NULL},
  {"path", (getter)Cookie_get_path, (setter)Cookie_set_path, "path", NULL},
  {"expired", (getter)Cookie_get_expired, (setter)Cookie_set_expired, "expired", NULL},
  {"max_age", (getter)Cookie_get_max_age, (setter)Cookie_set_max_age, "max_age", NULL},
  {"secure", (getter)Cookie_get_secure, (setter)Cookie_set_secure, "secure", NULL},
  {"httponly", (getter)Cookie_get_httponly, (setter)Cookie_set_httponly, "httponly", NULL},
  {NULL}
};

static PyMemberDef Cookie_members[] = {
  {NULL}
};

PyTypeObject CookieType = {
  PyObject_HEAD_INIT(NULL)
  0,                          /* ob_size */
  "cookie.Cookie",              /* tp_name */
  sizeof(Cookie),             /* tp_basicsize */
  0,                          /* tp_itemsize */
  (destructor)Cookie_dealloc, /* tp_dealloc */
  0,                          /* tp_print */
  0,                          /* tp_getattr */
  0,                          /* tp_setattr */
  0,                          /* tp_compare */
  (reprfunc)Cookie_repr,      /* tp_repr */
  0,                          /* tp_as_number */
  0,                          /* tp_as_sequence */
  0,                          /* tp_as_mapping */
  0,                          /* tp_hash */
  0,                          /* tp_call */
  0,                          /* tp_str */
  0,                          /* tp_getattro */
  0,                          /* tp_setattro */
  0,                          /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,         /* tp_flags */
  "Cookie objects",           /* tp_doc*/
  0,                          /* tp_traverse */
  0,                          /* tp_clear */
  0,                          /* tp_richcompare */
  0,                          /* tp_weaklistoffset */
  0,                          /* tp_iter */
  0,                          /* tp_iternext */
  0,                          /* tp_methods */
  Cookie_members,             /* tp_members */
  Cookie_getseter,            /* tp_getset */
  0,                          /* tp_base */
  0,                          /* tp_dict */
  0,                          /* tp_descr_get */
  0,                          /* tp_descr_set */
  0,                          /* tp_dictoffset */
  (initproc)Cookie_init,      /* tp_init */
  0,                          /* tp_alloc */
  Cookie_new,                 /* tp_new */
};

static PyMethodDef cookie_module_methods[] = {
  {NULL}
};

PyObject *init_cookie(void) {
  PyObject* m;

  if (PyType_Ready(&CookieType) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.cookie", cookie_module_methods, "guava.cookie .");

  if (!m) {
    return NULL;
  }

  Py_INCREF(&CookieType);

  PyModule_AddObject(m, "Cookie", (PyObject *)&CookieType);

  return m;
}
