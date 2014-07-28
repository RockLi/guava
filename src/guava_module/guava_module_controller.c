/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava.h"
#include "guava_module.h"
#include "guava_response.h"
#include "guava_session/guava_session.h"
#include "guava_cookie.h"
#include "guava_memory.h"

static PyObject *Controller_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  Controller *self;

  self = (Controller *)type->tp_alloc(type, 0);
  if (self) {
    self->resp = NULL;
    self->owned_resp = NULL;
    self->req = NULL;
    self->owned_req = NULL;
    self->SESSION = NULL;
  }

  return (PyObject *)self;
}

static void Controller_dealloc(Controller *self) {
  if (self->owned_resp) {
    Py_DECREF(self->owned_resp);
    self->owned_resp = NULL;
  }
  self->resp = NULL;

  if (self->owned_req) {
    Py_DECREF(self->owned_req);
    self->owned_req = NULL;
  }
  self->req = NULL;

  if (self->SESSION) {
    Py_DECREF(self->SESSION);
    self->SESSION = NULL;
  }

  self->ob_type->tp_free((PyObject *)self);
}

static int Controller_init(Controller *self, PyObject *args, PyObject *kwds) {
  PyObject *resp = NULL;
  PyObject *req = NULL;
  if (!PyArg_ParseTuple(args, "|OO", &resp, &req)) {
    return -1;
  }

  if (resp) {
    Py_INCREF(resp);
    self->resp = ((Response *)resp)->resp;
    self->owned_resp = resp;
  }

  if (req) {
    Py_INCREF(req);
    self->req = ((Request *)req)->req;
    self->owned_req = req;
  }

  self->SESSION = NULL;
  return 0;
}

static PyObject *Controller_set_header(Controller *self, PyObject *args) {
  guava_response_t *resp = self->resp;

  char *key;
  char *value;

  if (!PyArg_ParseTuple(args, "ss", &key, &value)) {
    PyErr_SetString(PyExc_TypeError, "give correct parameters");
    return NULL;
  }

  guava_response_set_header(resp, key, value);
  Py_RETURN_TRUE;
}

static PyObject *Controller_set_cookie(Controller *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"name", "value", "path", "domain", "secure", "httponly", "expired", "max_age", NULL};

  guava_response_t *resp = self->resp;

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
    return NULL;
  }

  PyObject *cookie = guava_cookie_new_object(name,
                                             value,
                                             path,
                                             domain,
                                             expired,
                                             max_age,
                                             secure,
                                             httponly);


  guava_response_set_cookie(resp, name, (PyObject *)cookie);

  Py_DECREF(cookie);

  Py_RETURN_TRUE;
}

static PyObject *Controller_write(Controller *self, PyObject *args) {
  guava_response_t *resp = self->resp;

  char *data;
  if (!PyArg_ParseTuple(args, "s", &data)) {
    PyErr_SetString(PyExc_TypeError, "error parameter");
    return NULL;
  }

  guava_response_write_data(resp, data);

  Py_RETURN_TRUE;
}

static PyObject *Controller_set_status_code(Controller *self, PyObject *args) {
  guava_response_t *resp = self->resp;

  int status_code;

  if (!PyArg_ParseTuple(args, "i", &status_code)) {
    PyErr_SetString(PyExc_TypeError, "error parameter");
    return NULL;
  }

  /* check the status code range */
  guava_response_set_status_code(resp, status_code);

  Py_RETURN_TRUE;
}

static PyObject *Controller_redirect(Controller *self, PyObject *args) {
  guava_response_t *resp = self->resp;

  char *url = NULL;
  if (!PyArg_ParseTuple(args, "s", &url)) {
    PyErr_SetString(PyExc_TypeError, "error parameter");
    return NULL;
  }

  if (!url) {
    Py_RETURN_FALSE;
  }

  guava_response_302(resp, url);

  Py_RETURN_TRUE;
}

static PyObject *Controller_before_action(Controller *self, PyObject *args) {
  Py_RETURN_NONE;
}

static PyObject *Controller_after_action(Controller *self, PyObject *args) {
  Py_RETURN_NONE;
}

static PyObject *Controller_get_SESSION(Controller *self, void *closure) {
  if (self->SESSION == NULL) {
    if (self->router->session_store) {
      Cookie *id = NULL;
      PyObject *v = Py_None;

      if (self->req && self->req->COOKIES) {
        id = (Cookie *)PyDict_GetItemString(self->req->COOKIES, self->router->session_store->name);
      }

      if (id) {
        v = guava_session_get(self->router->session_store, (guava_session_id_t)id->data.value);
      }

      if (v == Py_None) {
        self->SESSION = PyDict_New();
      } else {
        self->SESSION = v;
      }

    } else {
      PyErr_SetString(PyExc_TypeError, "you didn't enable the session for this router");
      return NULL;
    }
  }

  Py_INCREF(self->SESSION);

  return self->SESSION;
}

static PyObject *Controller_get_GET(Controller *self, void *closure) {
  if (!self->req) {
    return PyDict_New();
  }

  if (!self->req->GET) {
    self->req->GET = PyDict_New();
  }

  Py_INCREF(self->req->GET);

  return self->req->GET;
}

static PyObject *Controller_get_POST(Controller *self, void *closure) {
  if (!self->req) {
    return PyDict_New();
  }

  if (!self->req->POST) {
    self->req->POST = PyDict_New();
  }

  Py_INCREF(self->req->POST);

  return self->req->POST;
}

static PyObject *Controller_get_COOKIES(Controller *self, void *closure) {
  if (!self->req) {
    return PyDict_New();
  }

  if (!self->req->COOKIES) {
    self->req->COOKIES = PyDict_New();
  }

  Py_INCREF(self->req->COOKIES);

  return self->req->COOKIES;
}

static PyObject *Controller_get_HEADERS(Controller *self, void *closure) {
  if (!self->req) {
    return PyDict_New();
  }

  if (!self->req->headers) {
    self->req->headers = PyDict_New();
  }

  Py_INCREF(self->req->headers);

  return self->req->headers;
}

static PyGetSetDef Controller_getseter[] = {
  {"COOKIES", (getter)Controller_get_COOKIES, NULL, "COOKIES", NULL},
  {"GET", (getter)Controller_get_GET, NULL, "GET", NULL},
  {"POST", (getter)Controller_get_POST, NULL, "POST", NULL},
  {"SESSION", (getter)Controller_get_SESSION, NULL, "SESSION", NULL},
  {"HEADERS", (getter)Controller_get_HEADERS, NULL, "HEADERS", NULL},
  {NULL}
};

static PyMemberDef Controller_members[] = {
  {NULL}
};

static PyMethodDef Controller_methods[] = {
  {"set_header", (PyCFunction)Controller_set_header, METH_VARARGS, "set the response header"},
  {"set_cookie", (PyCFunction)Controller_set_cookie, METH_VARARGS, "set the response cookie"},
  {"write", (PyCFunction)Controller_write, METH_VARARGS, "write the data to client"},
  {"set_status_code", (PyCFunction)Controller_set_status_code, METH_VARARGS, "set the response status code"},
  {"redirect", (PyCFunction)Controller_redirect, METH_VARARGS, "redirect to another url"},
  {"before_action", (PyCFunction)Controller_before_action, METH_NOARGS, "called before execute the action"},
  {"after_action", (PyCFunction)Controller_after_action, METH_NOARGS, "called after executed the action"},
  {NULL}
};

PyTypeObject ControllerType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "controller.Controller",                  /* tp_name */
  sizeof(Controller),                       /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor)Controller_dealloc,           /* tp_dealloc */
  0,                                        /* tp_print */
  0,                                        /* tp_getattr */
  0,                                        /* tp_setattr */
  0,                                        /* tp_compare */
  0,                                        /* tp_repr */
  0,                                        /* tp_as_number */
  0,                                        /* tp_as_sequence */
  0,                                        /* tp_as_mapping */
  0,                                        /* tp_hash */
  0,                                        /* tp_call */
  0,                                        /* tp_str */
  0,                                        /* tp_getattro */
  0,                                        /* tp_setattro */
  0,                                        /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
  "Controller objects",                     /* tp_doc*/
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  0,                                        /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  Controller_methods,                       /* tp_methods */
  Controller_members,                       /* tp_members */
  Controller_getseter,                      /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  (initproc)Controller_init,                /* tp_init */
  0,                                        /* tp_alloc */
  Controller_new,                           /* tp_new */
};

static PyMethodDef controller_module_methods[] = {
  {NULL}
};

PyObject *init_controller(void) {
  PyObject* m;

  if (PyType_Ready(&ControllerType) < 0) {
    return NULL;
  }

  m = Py_InitModule3("guava.controller", controller_module_methods, "guava.controller .");

  if (!m) {
    return NULL;
  }

  Py_INCREF(&ControllerType);

  PyModule_AddObject(m, "Controller", (PyObject *)&ControllerType);
  return m;
}
