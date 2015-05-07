/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_cookie.h"
#include "guava_string.h"
#include "guava_module.h"
#include "guava_memory.h"

static PyObject *
Py_Cookie_new(PyTypeObject *type,
              PyObject     *args,
              PyObject     *kwds)
{
  Py_Cookie *self;

  self = (Py_Cookie *)type->tp_alloc(type, 0);
  if (self) {
    self->cookie = NULL;
  }

  return (PyObject *)self;
}

static void
Py_Cookie_dealloc(Py_Cookie *self)
{
  if (self->cookie) {
    guava_cookie_destroy(self->cookie);
  }
  self->ob_type->tp_free((PyObject *)self);
}

static int
Py_Cookie_init(Py_Cookie *self,
               PyObject  *args,
               PyObject  *kwds)
{
  static char *kwlist[] = {"name", "value", "path", "domain", "expired", "max_age", "secure", "httponly", NULL};

  char *name     = NULL;
  char *value    = NULL;
  char *path     = NULL;
  char *domain   = NULL;
  int   expired  = -1;
  int   max_age  = -1;
  int   secure   = GUAVA_FALSE;
  int   httponly = GUAVA_FALSE;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "ss|ssiiBB",
                                   kwlist,
                                   &name,
                                   &value,
                                   &path,
                                   &domain,
                                   &expired,
                                   &max_age,
                                   &secure,
                                   &httponly)) {
    return -1;
  }

  self->cookie = guava_cookie_new(name,
                                  value,
                                  path,
                                  domain,
                                  expired,
                                  max_age,
                                  secure,
                                  httponly);

  if (!self->cookie) {
    PyErr_SetString(PyExc_TypeError, "failed to initialize a new cookie object");
    return -1;
  }

  return 0;
}

static PyObject *
Py_Cookie_repr(Py_Cookie *self) {
  return PyString_FromFormat("%s", "cookie");
}

static PyObject *
Py_Cookie_get_name(Py_Cookie *self,
                   void   *closure)
{
  if (self->cookie->name) {
    return PyString_FromString(self->cookie->name);
  }

  Py_RETURN_NONE;
}

static int
Py_Cookie_set_name(Py_Cookie *self,
                   PyObject  *value,
                   void      *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the name property");

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The name attribute value must be a string");
    return -1;
  }

  if (!guava_cookie_set_name(self->cookie, PyString_AsString(value))) {
   PyErr_SetString(PyExc_TypeError, "failed to set the name attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_Cookie_get_value(Py_Cookie *self,
                    void      *closure)
{
  if (self->cookie->value) {
    return PyString_FromString(self->cookie->value);
  }

  Py_RETURN_NONE;
}

static int
Py_Cookie_set_value(Py_Cookie *self,
                    PyObject  *value,
                    void      *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the value property");

  if (!PyString_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The value attribute value must be a string");
    return -1;
  }

  if (!guava_cookie_set_value(self->cookie, PyString_AsString(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the value attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_Cookie_get_path(Py_Cookie *self,
                   void      *closure)
{
  if (self->cookie->path) {
    return PyString_FromString(self->cookie->path);
  }

  Py_RETURN_NONE;
}

static int
Py_Cookie_set_path(Py_Cookie *self,
                   PyObject  *value,
                   void      *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the path property");

  if (!PyString_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "The path attribute value must be a string or None");
    return -1;
  }

  if (guava_cookie_set_path(self->cookie, Py_None != value ? PyString_AsString(value) : NULL)) {
    PyErr_SetString(PyExc_TypeError, "failed to set the path attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_Cookie_get_domain(Py_Cookie *self,
                     void      *closure)
{
  if (self->cookie->domain) {
    return PyString_FromString(self->cookie->domain);
  }

  Py_RETURN_NONE;
}

static int
Py_Cookie_set_domain(Py_Cookie *self,
                     PyObject  *value,
                     void      *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the domain property");

  if (!PyString_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "The domain attribute value must be a string or None");
    return -1;
  }

  if (!guava_cookie_set_domain(self->cookie, Py_None != value ? PyString_AsString(value) : NULL)) {
    PyErr_SetString(PyExc_TypeError, "failed to set the domain attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_Cookie_get_expired(Py_Cookie *self,
                      void      *closure)
{
  return PyInt_FromLong(self->cookie->expired);
}

static int
Py_Cookie_set_expired(Py_Cookie *self,
                      PyObject  *value,
                      void      *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the password property");

  if (!PyInt_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The expired attribute value must be a int");
    return -1;
  }

  if (!guava_cookie_set_expired(self->cookie, (int)PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the expired attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_Cookie_get_max_age(Py_Cookie *self,
                      void      *closure)
{
  return PyInt_FromLong(self->cookie->max_age);
}

static int
Py_Cookie_set_max_age(Py_Cookie *self,
                      PyObject  *value,
                      void      *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the max_age property");

  if (!PyInt_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The max_age attribute value must be a string");
    return -1;
  }

  if (!guava_cookie_set_max_age(self->cookie, (int)PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the max_age attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_Cookie_get_secure(Py_Cookie *self,
                     void      *closure)
{
  return PyBool_FromLong(self->cookie->secure);
}

static int
Py_Cookie_set_secure(Py_Cookie *self,
                     PyObject  *value,
                     void      *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the secure property");

  if (!PyBool_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The secure attribute value must be a boolean");
    return -1;
  }

  if (!guava_cookie_set_secure(self->cookie, PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the secure attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_Cookie_get_httponly(Py_Cookie *self,
                       void      *closure)
{
  return PyBool_FromLong(self->cookie->httponly);
}

static int
Py_Cookie_set_httponly(Py_Cookie *self,
                       PyObject  *value,
                       void      *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the httponly property");

  if (!PyBool_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The httponly attribute value must be a boolean");
    return -1;
  }

  if (!guava_cookie_set_httponly(self->cookie, PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the httponly attribute");
    return -1;
  }

  return 0;
}

static PyGetSetDef Py_Cookie_getseter[] = {
  {"name", (getter)Py_Cookie_get_name, (setter)Py_Cookie_set_name, "name", NULL},
  {"value", (getter)Py_Cookie_get_value, (setter)Py_Cookie_set_value, "value", NULL},
  {"path", (getter)Py_Cookie_get_path, (setter)Py_Cookie_set_path, "path", NULL},
  {"domain", (getter)Py_Cookie_get_domain, (setter)Py_Cookie_set_domain, "domain", NULL},
  {"expired", (getter)Py_Cookie_get_expired, (setter)Py_Cookie_set_expired, "expired", NULL},
  {"max_age", (getter)Py_Cookie_get_max_age, (setter)Py_Cookie_set_max_age, "max_age", NULL},
  {"secure", (getter)Py_Cookie_get_secure, (setter)Py_Cookie_set_secure, "secure", NULL},
  {"httponly", (getter)Py_Cookie_get_httponly, (setter)Py_Cookie_set_httponly, "httponly", NULL},
  {NULL}
};

static PyMemberDef Py_Cookie_members[] = {
  {NULL}
};

PyTypeObject PyType_Cookie = {
  PyObject_HEAD_INIT(NULL)
  0,                             /* ob_size */
  "cookie.Cookie",               /* tp_name */
  sizeof(Py_Cookie),             /* tp_basicsize */
  0,                             /* tp_itemsize */
  (destructor)Py_Cookie_dealloc, /* tp_dealloc */
  0,                             /* tp_print */
  0,                             /* tp_getattr */
  0,                             /* tp_setattr */
  0,                             /* tp_compare */
  (reprfunc)Py_Cookie_repr,      /* tp_repr */
  0,                             /* tp_as_number */
  0,                             /* tp_as_sequence */
  0,                             /* tp_as_mapping */
  0,                             /* tp_hash */
  0,                             /* tp_call */
  0,                             /* tp_str */
  0,                             /* tp_getattro */
  0,                             /* tp_setattro */
  0,                             /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,            /* tp_flags */
  "Cookie objects",              /* tp_doc*/
  0,                             /* tp_traverse */
  0,                             /* tp_clear */
  0,                             /* tp_richcompare */
  0,                             /* tp_weaklistoffset */
  0,                             /* tp_iter */
  0,                             /* tp_iternext */
  0,                             /* tp_methods */
  Py_Cookie_members,             /* tp_members */
  Py_Cookie_getseter,            /* tp_getset */
  0,                             /* tp_base */
  0,                             /* tp_dict */
  0,                             /* tp_descr_get */
  0,                             /* tp_descr_set */
  0,                             /* tp_dictoffset */
  (initproc)Py_Cookie_init,      /* tp_init */
  0,                             /* tp_alloc */
  Py_Cookie_new,                 /* tp_new */
};

static PyMethodDef Py_cookie_module_methods[] = {
  {NULL}
};

PyObject *
init_cookie_module(void)
{
  PyObject* m = NULL;
  if (PyType_Ready(&PyType_Cookie) < 0) return NULL;

  m = Py_InitModule3("guava.cookie", Py_cookie_module_methods, "guava.cookie .");
  if (!m) return NULL;

  Py_INCREF(&PyType_Cookie);

  PyModule_AddObject(m, "Cookie", (PyObject *)&PyType_Cookie);
  return m;
}
