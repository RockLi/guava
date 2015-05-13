/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_memory.h"
#include "guava_url.h"
#include "guava_session/guava_session_handler.h"
#include "guava_module/guava_module.h"
#include "guava_module/guava_module_session_store.h"
#include "marshal.h"


guava_string_t
get_session_file_path(Py_FileSessionStore *self,
                      const char          *sid)
{
  if (!self || !sid) return NULL;

  char path[MAXPATH] = {0};
  snprintf(path, sizeof(path), "%s/%s", self->directory, sid);
  return guava_string_new(path);
}

static PyObject *
Py_FileSessionStore_read(Py_FileSessionStore *self,
                         PyObject            *args)
{
  const char *sid = NULL;
  if (!PyArg_ParseTuple(args, "s", &sid)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  guava_string_t path = get_session_file_path(self, sid);
  if (!path) {
    Py_RETURN_NONE;
  }
  FILE *fp = fopen(path, "rb");
  guava_string_free(path);

  if (NULL == fp) {
    PyErr_SetString(PyExc_TypeError, "failed to open the file, please confirm the session directory exists and you have right permissions");
    Py_RETURN_NONE;
  }

  /* @Todo: Intro lock to protect here */
  PyObject *v = PyMarshal_ReadObjectFromFile(fp);
  fclose(fp);

  if (NULL == v) {
    Py_RETURN_NONE;
  }

  return v;
}

static PyObject *
Py_FileSessionStore_write(Py_FileSessionStore *self,
                          PyObject            *args)
{
  const char *sid = NULL;
  PyObject     *v = NULL;
  if (!PyArg_ParseTuple(args, "sO", &sid, &v)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  if (!PyDict_Check(v)) {
    PyErr_SetString(PyExc_TypeError, "value must a dict object");
    return NULL;
  }

  guava_string_t path = get_session_file_path(self, sid);
  if (!path) {
    Py_RETURN_NONE;
  }

  FILE *fp = fopen(path, "wb");
  guava_string_free(path);

  if (NULL == fp) {
    PyErr_SetString(PyExc_TypeError, "failed to open the file, please confirm the session directory exists and you have right permissions");
    return NULL;
  }

  PyMarshal_WriteObjectToFile(v, fp, Py_MARSHAL_VERSION);
  fclose(fp);

  Py_RETURN_NONE;
}

static PyObject *
Py_FileSessionStore_destroy(Py_FileSessionStore *self,
                            PyObject            *args)
{
  const char *sid = NULL;
  if (!PyArg_ParseTuple(args, "s", &sid)) {
    PyErr_SetString(PyExc_TypeError, "type error");
    return NULL;
  }

  guava_string_t path = get_session_file_path(self, sid);
  if (!path) {
    Py_RETURN_NONE;
  }

  unlink(path);
  guava_string_free(path);

  Py_RETURN_NONE;
}

static PyMethodDef Py_FileSessionStore_methods[] = {
    {"read", (PyCFunction)Py_FileSessionStore_read, METH_VARARGS, PyDoc_STR("read the data")},
    {"write", (PyCFunction)Py_FileSessionStore_write, METH_VARARGS, PyDoc_STR("write the data")},
    {"destroy", (PyCFunction)Py_FileSessionStore_destroy, METH_VARARGS, PyDoc_STR("destroy the data")},
    {NULL,	NULL},
};

static int
Py_FileSessionStore_init(Py_FileSessionStore *self,
                         PyObject            *args,
                         PyObject            *kwds)
{
  const char *directory = NULL;

  PyObject *o_directory = PyDict_GetItemString(kwds, "directory");
  if (!o_directory || Py_None == o_directory) {
    directory = "/tmp";
  } else {
    if (!PyString_Check(o_directory)) {
      PyErr_SetString(PyExc_TypeError, "the directory must be a string or None");
      return -1;
    }
    directory = PyString_AsString(o_directory);
  }

  /* Del the directory attibute before pass to the base class */
  PyDict_DelItemString(kwds, "directory");

  if (PyType_SessionStore.tp_init((PyObject *)self, args, kwds) < 0) {
        return -1;
  }

  /* PyObject_GenericSetAttr((PyObject *)self, */
  /*                         PyString_FromFormat("directory"), */
  /*                         PyString_FromFormat("%s", directory)); */
  self->directory = guava_string_new(directory);
  return 0;
}

PyTypeObject PyType_FileSessionStore = {
    PyObject_HEAD_INIT(NULL)
    0,                                      /* ob_size */
    "session.FileSessionStore",             /* tp_name */
    sizeof(Py_FileSessionStore),            /* tp_basicsize */
    0,                                      /* tp_itemsize */
    0,                                      /* tp_dealloc */
    0,                                      /* tp_print */
    0,                                      /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_compare */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    0,                                      /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, /* tp_flags */
    0,                                      /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    Py_FileSessionStore_methods,            /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)Py_FileSessionStore_init,     /* tp_init */
    0,                                      /* tp_alloc */
    0,                                      /* tp_new */
};
