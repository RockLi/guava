/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_response.h"
#include "guava_string.h"
#include "guava_module.h"
#include "guava_memory.h"

static guava_status_code_t guava_status_codes[] = {
  {100, "Continue"},
  {101, "Switching Protocols"},
  {102, "Processing"},

  {200, "OK"},
  {201, "Created"},
  {202, "Accepted"},
  {203, "Non-Authoritative Information"},
  {204, "No Content"},
  {205, "Reset Content"},
  {206, "Partial Content"},
  {207, "Multi-Status"},
  {208, "Already Reported"},
  {226, "IM Used"},

  {300, "Multiple Choices"},
  {301, "Moved Permanently"},
  {302, "Found"},
  {303, "See Other"},
  {304, "Not Modified"},
  {305, "Use Proxy"},
  {306, "Switch Proxy"},
  {307, "Temporary Redirect"},
  {308, "Permanent Redirect"},

  {400, "Bad Request"},
  {401, "Unauthorized"},
  {402, "Payment Required"},
  {403, "Forbidden"},
  {404, "Not Found"},
  {405, "Method Not Allowed"},
  {406, "Not Acceptable"},
  {407, "Proxy Authentication Required"},
  {408, "Request Timeout"},
  {409, "Conflict"},
  {410, "Gone"},
  {411, "Length Required"},
  {412, "Precondition Failed"},
  {413, "Request Entity Too Large"},
  {414, "Request-URI Too Long"},
  {415, "Unsupported Media Type"},
  {416, "Requested Range Not Satisfiable"},
  {417, "Expectation Failed"},
  {418, "I'm a teapot"},
  {419, "Authentication Timeout"},
  {422, "Unprocessable Entity"},
  {423, "Locked"},
  {424, "Failed Dependency"},
  {426, "Upgrade Required"},
  {428, "Precondition Required"},
  {429, "Too Many Requests"},
  {431, "Request Header Fields Too Large"},
  {440, "Login Timeout"},
  {444, "No Response"},
  {449, "Retry With"},
  {450, "Blocked by Windows Parental Controls"},
  {451, "Redirect"},
  {494, "Request Header Too Large"},
  {495, "Cert Error"},
  {496, "No Cert"},
  {497, "HTTP to HTTPS"},
  {498, "Token expired/invalid"},
  {499, "Client Closed Request"},
  {499, "Token required"},

  {500, "Internal Server Error"},
  {501, "Not Implemented"},
  {502, "Bad Gateway"},
  {503, "Service Unavailable"},
  {504, "Gateway Timeout"},
  {505, "HTTP Version Not Supported"},
  {506, "Variant Also Negotiates"},
  {507, "Insufficient Storage"},
  {508, "Loop Detected"},
  {509, "Bandwidth Limit Exceeded"},
  {510, "Not Extended"},
  {511, "Network Authentication Required"},
  {520, "Origin Error"}
};

guava_response_t *guava_response_new(void) {
  guava_response_t *resp = (guava_response_t *)guava_malloc(sizeof(guava_response_t));
  if (!resp) {
    return NULL;
  }

  resp->major = 1;
  resp->minor = 1;
  resp->status_code = 200;
  resp->data = NULL;
  resp->headers = PyDict_New();
  resp->serialized_data = NULL;
  resp->cookies = NULL;

  guava_response_set_header(resp, "Server", SERVER_NAME);

  return resp;
}

void guava_response_free(guava_response_t *resp) {
  if (resp->data) {
    guava_string_free(resp->data);
  }

  if (resp->headers) {
    Py_DECREF(resp->headers);
  }

  if (resp->serialized_data) {
    guava_string_free(resp->serialized_data);
  }

  if (resp->cookies) {
    Py_DECREF(resp->cookies);
  }

  guava_free(resp);
}

void guava_response_set_conn(guava_response_t *resp, guava_conn_t *conn) {
  resp->conn = conn;
}

void guava_response_set_version(guava_response_t *resp, uint16_t major, uint16_t minor) {
  resp->major = major;
  resp->minor = minor;
}

void guava_response_set_status_code(guava_response_t *resp, uint16_t status_code) {
  resp->status_code = status_code;
}

void guava_response_set_header(guava_response_t *resp, const char *key, const char *value) {
  PyObject *v = PyString_FromString(value);
  PyDict_SetItemString(resp->headers, key, v);
  Py_DECREF(v);
}

void guava_response_set_cookie(guava_response_t *resp, const char *key, PyObject *value) {
  if (!resp->cookies) {
    resp->cookies = PyDict_New();
  }

  PyDict_SetItemString(resp->cookies, key, value);
}

void guava_response_set_data(guava_response_t *resp, guava_string_t data) {
  resp->data = data;
}

void guava_response_write_data(guava_response_t *resp, const char *data) {
  if (!data) {
    return;
  }
  resp->data = guava_string_append_raw(resp->data, data);
}

const char *guava_status_code_desc(int code) {
  for (size_t i = 0; i < sizeof(guava_status_codes) / sizeof(guava_status_codes[0]); ++i) {
    if (guava_status_codes[i].code == code) {
      return guava_status_codes[i].desc;
    }
  }

  return NULL;
}

guava_string_t guava_response_serialize(guava_response_t *resp) {
  char buf[1024];
  snprintf(buf, sizeof(buf), "HTTP/%d.%d %d %s\r\n",
           resp->major,
           resp->minor,
           resp->status_code,
           guava_status_code_desc(resp->status_code));

  guava_string_t s = guava_string_append_raw(NULL, buf);

  PyObject *key, *value;
  Py_ssize_t pos = 0;
  while (PyDict_Next(resp->headers, &pos, &key, &value)) {
    snprintf(buf, sizeof(buf), "%s: %s\r\n", PyString_AsString(key), PyString_AsString(value));
    s = guava_string_append_raw(s, buf);
  }

  PyObject *kkey = PyString_FromString("Content-Length");
  if (!PyDict_Contains(resp->headers, kkey)) {
    snprintf(buf, sizeof(buf), "Content-Length: %zu\r\n", resp->data ? guava_string_len(resp->data) : 0);
    s = guava_string_append_raw(s, buf);
  }
  Py_DECREF(kkey);

  kkey = PyString_FromString("Set-Cookie");
  if (!PyDict_Contains(resp->headers, kkey) && resp->cookies) {
    PyObject *cookie_key = NULL;
    PyObject *cookie_value = NULL;
    Cookie *cookie = NULL;
    Py_ssize_t cookie_pos = 0;
    while (PyDict_Next(resp->cookies, &cookie_pos, &cookie_key, &cookie_value)) {
      cookie = (Cookie *)cookie_value;
      s = guava_string_append_raw(s, "Set-Cookie: ");
      s = guava_string_append_raw(s, cookie->data.name);
      s = guava_string_append_raw(s, "=");
      s = guava_string_append_raw(s, cookie->data.value);
      if (cookie->data.domain) {
        s = guava_string_append_raw(s, " ;Domain=");
        s = guava_string_append_raw(s, cookie->data.domain);
      }
      if (cookie->data.path) {
        s = guava_string_append_raw(s, " ;Path=");
        s = guava_string_append_raw(s, cookie->data.path);
      }
      if (cookie->data.expired >= 0) {
        s = guava_string_append_raw(s, " ;Expires=");
        s = guava_string_append_int(s, cookie->data.expired);
      }
      if (cookie->data.max_age >= 0) {
        s = guava_string_append_raw(s, " ;Max-Age=");
        s = guava_string_append_int(s, cookie->data.max_age);
      }
      if (cookie->data.secure) {
        s = guava_string_append_raw(s, " ;Secure");
      }
      if (cookie->data.httponly) {
        s = guava_string_append_raw(s, " ;HttpOnly");
      }
      s = guava_string_append_raw(s, "\r\n");
    }
  }
  Py_DECREF(kkey);

  s = guava_string_append_raw(s, "\r\n");

  if (resp->data) {
    s = guava_string_append_raw(s, resp->data);
  }

  resp->serialized_data = s;
  return s;
}

void guava_response_send(guava_response_t *resp, uv_write_cb cb) {
  Request *request = (Request *)resp->conn->request;
  if (request->req->keep_alive) {
    guava_response_set_header(resp, "Connection", request->req->keep_alive ? "keep-alive" : "close");
  }

  PyObject *key = PyString_FromString("Content-Type");
  if (!PyDict_Contains(resp->headers, key)) {
    guava_response_set_header(resp, "Content-Type", "text/html");
  }
  Py_DECREF(key);

  guava_string_t data = guava_response_serialize(resp);
  uv_buf_t b = uv_buf_init(data, (unsigned int)guava_string_len(data));

  resp->conn->write_req.data = resp;
  uv_write(&resp->conn->write_req, (uv_stream_t *)&resp->conn->stream, &b, 1, cb);
}

void guava_response_404(guava_response_t *resp, void *closure) {
  guava_response_set_status_code(resp, 404);
  guava_response_set_data(resp, guava_string_new("404 Not Found!"));
}

void guava_response_500(guava_response_t *resp, void *closure) {
  guava_response_set_status_code(resp, 500);
  guava_response_set_data(resp, guava_string_new("500 Internal Server Error!"));
}

void guava_response_302(guava_response_t *resp, void *closure) {
  guava_response_set_status_code(resp, 303);
  const char *url = (const char *)closure;
  guava_response_set_header(resp, "Location", url);
}
