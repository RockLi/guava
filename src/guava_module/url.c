/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_module.h"
#include "guava_memory.h"
#include "guava_url.h"

static PyObject *
Py_URL_new(PyTypeObject *type,
           PyObject     *args,
           PyObject     *kwds)
{
  Py_URL *self;

  self = (Py_URL *)type->tp_alloc(type, 0);
  if (self) {
    self->url = NULL;
  }

  return (PyObject *)self;
}

static void
Py_URL_dealloc(Py_URL *self)
{
  guava_url_destroy(self->url);
  self->ob_type->tp_free((PyObject *)self);
}

static int
Py_URL_init(Py_URL   *self,
            PyObject *args,
            PyObject *kwds)
{
  static char *kwlist[] = {"schema", "username", "password", "host", "port", "path", "queries", "fragment", NULL};

  char     *schema    = NULL;
  char     *username  = NULL;
  char     *password  = NULL;
  char     *host      = NULL;
  int       port      = 80;
  char     *path      = NULL;
  PyObject *queries   = NULL;
  char     *fragment  = NULL;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "|ssssisOs",
                                   kwlist,
                                   &schema,
                                   &username,
                                   &password,
                                   &host,
                                   &port,
                                   &path,
                                   &queries,
                                   &fragment)) {
    return -1;
  }

  self->url = guava_url_new(schema ? schema : "http",
                            username,
                            password,
                            host ? host : "localhost",
                            port,
                            path ? path : "/",
                            NULL,
                            fragment);

  if (queries) {
    PyObject *key = NULL, *value = NULL;
    Py_ssize_t pos = 0;
    while (PyDict_Next(queries, &pos, &key, &value)) {
      if (!PyString_Check(key) ||
          !PyString_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "the value of query must be string");
        return -1;
      }
      guava_url_set_query(self->url,
                          PyString_AsString(key),
                          PyString_AsString(value));
    }
  }

  return 0;
}

static PyObject *
Py_URL_repr(Py_URL *self)
{
  guava_string_t encoded_url = guava_url_encoded(self->url);
  if (encoded_url) {
    return PyString_FromFormat("%s", encoded_url);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject *
Py_URL_get_schema(Py_URL *self,
                  void   *closure)
{
  if (self->url->schema) {
    return PyString_FromString(self->url->schema);
  } else {
    Py_RETURN_NONE;
  }
}

static int
Py_URL_set_schema(Py_URL   *self,
                  PyObject *value,
                  void     *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the schema property");

  if (!PyString_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "the schema attribute must be string or None");
    return -1;
  }

  if (!guava_url_set_schema(self->url, Py_None != value ? PyString_AsString(value) : NULL)) {
    PyErr_SetString(PyExc_TypeError, "failed to set the schema attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_URL_get_username(Py_URL *self,
                    void   *closure)
{
  if (self->url->username) {
    return PyString_FromString(self->url->username);
  } else {
    Py_RETURN_NONE;
  }
}

static int
Py_URL_set_username(Py_URL   *self,
                    PyObject *value,
                    void     *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the username property");

  if (!PyString_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "the username attribute must be string or None");
    return -1;
  }

  if (!guava_url_set_username(self->url, Py_None != value ? PyString_AsString(value) : NULL)) {
    PyErr_SetString(PyExc_TypeError, "failed to set the username attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_URL_get_password(Py_URL *self,
                    void   *closure)
{
  if (self->url->password) {
    return PyString_FromString(self->url->password);
  } else {
    Py_RETURN_NONE;
  }
}

static int
Py_URL_set_password(Py_URL   *self,
                    PyObject *value,
                    void     *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the password property");

  if (!PyString_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "the password attribute must be string or None");
    return -1;
  }

  if (!guava_url_set_password(self->url, Py_None != value ? PyString_AsString(value) : NULL)) {
    PyErr_SetString(PyExc_TypeError, "failed to set the password attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_URL_get_host(Py_URL *self,
                void   *closure)
{
  if (self->url->host) {
    return PyString_FromString(self->url->host);
  } else {
    Py_RETURN_NONE;
  }
}

static int
Py_URL_set_host(Py_URL   *self,
                PyObject *value,
                void     *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the host property");

  if (!PyString_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "the host attribute must be string or None");
    return -1;
  }

  if (!guava_url_set_host(self->url, Py_None != value ? PyString_AsString(value) : NULL)) {
    PyErr_SetString(PyExc_TypeError, "failed to set the host attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_URL_get_port(Py_URL *self,
                void   *closure)
{

  return PyInt_FromLong(self->url->port);
}

static int
Py_URL_set_port(Py_URL   *self,
                PyObject *value,
                void     *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the host property");

  if (!PyInt_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "the port attribute must be integer");
    return -1;
  }

  if (!guava_url_set_port(self->url, (uint16_t)PyInt_AsLong(value))) {
    PyErr_SetString(PyExc_TypeError, "failed to set the port attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_URL_get_path(Py_URL *self,
                void   *closure)
{
  if (self->url->path) {
    return PyString_FromString(self->url->path);
  } else {
    Py_RETURN_NONE;
  }
}

static int
Py_URL_set_path(Py_URL   *self,
                PyObject *value,
                void     *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the path property");

  if (!PyString_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "the path attribute must be string or None");
    return -1;
  }

  if (!guava_url_set_path(self->url, Py_None != value ? PyString_AsString(value) : NULL)) {
    PyErr_SetString(PyExc_TypeError, "failed to set the path attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_URL_get_queries(Py_URL *self,
                   void   *closure)
{
  if (self->url->queries) {
    PyObject *dict = PyDict_New();
    if (!dict) return NULL;

    guava_hashtable_iter_t *it = guava_hashtable_iter_new(self->url->queries);
    while (guava_hashtable_iter_valid(it)) {
      const char *key = guava_hashtable_iter_get_key(it);
      const char *value = (const char *)guava_hashtable_iter_get_value(it);
      PyDict_SetItemString(dict, key, PyString_FromString(value));
      if (guava_hashtable_iter_next(it) == GUAVA_FALSE) break;
    }
    guava_hashtable_iter_destroy(it);

    return dict;
  } else {
    Py_RETURN_NONE;
  }
}

static int
Py_URL_set_queries(Py_URL   *self,
                   PyObject *value,
                   void     *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the queries property");

  if (!PyDict_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "the queries attribute must be dict or None");
    return -1;
  }

  if (Py_None == value) {
    guava_url_remove_queries(self->url);
    return 0;
  }

  PyObject *dict_key = NULL, *dict_value = NULL;
  Py_ssize_t pos = 0;
  while (PyDict_Next(value, &pos, &dict_key, &dict_value)) {
    if (!PyString_Check(dict_key) || !PyString_Check(dict_value)) {
      guava_url_remove_queries(self->url);
      return -1;
    }

    if (!guava_url_set_query(self->url,
                             PyString_AsString(dict_key),
                             PyString_AsString(dict_value))) {
      guava_url_remove_queries(self->url);
      PyErr_SetString(PyExc_TypeError, "failed to set the queries attribute");
      return -1;
    }
  }

  return 0;
}

static PyObject *
Py_URL_get_fragment(Py_URL *self,
                    void   *closure)
{
  if (self->url->fragment) {
    return PyString_FromString(self->url->fragment);
  } else {
    Py_RETURN_NONE;
  }
}

static int
Py_URL_set_fragment(Py_URL   *self,
                    PyObject *value,
                    void     *closure)
{
  MODULE_SETTER_CHECK_PROPERTY_DELETE("can not delete the fragment property");

  if (!PyString_Check(value) &&
      Py_None != value) {
    PyErr_SetString(PyExc_TypeError, "the fragment attribute must be string or None");
    return -1;
  }

  if (!guava_url_set_fragment(self->url, Py_None != value ? PyString_AsString(value) : NULL)) {
    PyErr_SetString(PyExc_TypeError, "failed to set the fragment attribute");
    return -1;
  }

  return 0;
}

static PyObject *
Py_URL_set_query(Py_URL   *self,
                 PyObject *args)
{
  char *key = NULL;
  char *value = NULL;
  if (!PyArg_ParseTuple(args, "ss", &key, &value)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  if (!guava_url_set_query(self->url,
                           key,
                           value)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  Py_RETURN_TRUE;
}

static PyObject *
Py_URL_get_query(Py_URL   *self,
                 PyObject *args)
{
  char *key = NULL;
  if (!PyArg_ParseTuple(args, "s", &key)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  guava_string_t value = (guava_string_t)guava_url_get_query(self->url, key);
  return PyString_FromString(value);
}

static PyObject *
Py_URL_encoded(Py_URL   *self,
               PyObject *args)
{
  return PyString_FromString(guava_url_encoded(self->url));
}

static PyObject *
Py_URL_unencoded(Py_URL   *self,
               PyObject *args)
{
  return PyString_FromString(guava_url_unencoded(self->url));
}

static PyMemberDef Py_URL_members[] = {
  {NULL}
};

static PyMethodDef Py_URL_methods[] = {
  {"set_query", (PyCFunction)Py_URL_set_query, METH_VARARGS, "set the query"},
  {"get_query", (PyCFunction)Py_URL_get_query, METH_VARARGS, "set the query"},
  {"encoded", (PyCFunction)Py_URL_encoded, METH_VARARGS, "get the encoded representation of URL object"},
  {"unencoded", (PyCFunction)Py_URL_unencoded, METH_VARARGS, "get the unencoded representation of URL object"},
  {NULL}
};

static PyGetSetDef Py_URL_getseter[] = {
  {"schema", (getter)Py_URL_get_schema, (setter)Py_URL_set_schema, "get/set schema", NULL},
  {"username", (getter)Py_URL_get_username, (setter)Py_URL_set_username, "get/set username", NULL},
  {"password", (getter)Py_URL_get_password, (setter)Py_URL_set_password, "get/set password", NULL},
  {"host", (getter)Py_URL_get_host, (setter)Py_URL_set_host, "get/set host", NULL},
  {"port", (getter)Py_URL_get_port, (setter)Py_URL_set_port, "get/set port", NULL},
  {"path", (getter)Py_URL_get_path, (setter)Py_URL_set_path, "get/set path", NULL},
  {"queries", (getter)Py_URL_get_queries, (setter)Py_URL_set_queries, "get/set queries", NULL},
  {"fragment", (getter)Py_URL_get_fragment, (setter)Py_URL_set_fragment, "get/set fragment", NULL},
  {NULL}
};

PyTypeObject PyType_URL = {
  PyObject_HEAD_INIT(NULL)
  0,                          /* ob_size */
  "url.URL",                  /* tp_name */
  sizeof(Py_URL),             /* tp_basicsize */
  0,                          /* tp_itemsize */
  (destructor)Py_URL_dealloc, /* tp_dealloc */
  0,                          /* tp_print */
  0,                          /* tp_getattr */
  0,                          /* tp_setattr */
  0,                          /* tp_compare */
  (reprfunc)Py_URL_repr,      /* tp_repr */
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
  "URL objects",              /* tp_doc*/
  0,                          /* tp_traverse */
  0,                          /* tp_clear */
  0,                          /* tp_richcompare */
  0,                          /* tp_weaklistoffset */
  0,                          /* tp_iter */
  0,                          /* tp_iternext */
  Py_URL_methods,             /* tp_methods */
  Py_URL_members,             /* tp_members */
  Py_URL_getseter,            /* tp_getset */
  0,                          /* tp_base */
  0,                          /* tp_dict */
  0,                          /* tp_descr_get */
  0,                          /* tp_descr_set */
  0,                          /* tp_dictoffset */
  (initproc)Py_URL_init,      /* tp_init */
  0,                          /* tp_alloc */
  Py_URL_new,                 /* tp_new */
};

static PyMethodDef Py_URL_module_methods[] = {
  /* {"parse", (PyCFunction)py_url_parse, METH_VARARGS, "parse the URL from raw string"}, */
  {NULL}
};

PyObject *
init_url_module(void)
{
  PyObject* m = NULL;

  if (PyType_Ready(&PyType_URL) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.url", Py_URL_module_methods, "guava.url .");
  if (!m) {
    return NULL;
  }

  Py_INCREF(&PyType_URL);

  PyModule_AddObject(m, "URL", (PyObject *)&PyType_URL);
  return m;
}
