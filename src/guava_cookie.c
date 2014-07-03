/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_cookie.h"
#include "guava_string.h"
#include "guava_module.h"
#include "guava_memory.h"

PyObject *guava_cookie_new_object(const char  *name,
                                  const char  *value,
                                  const char  *path,
                                  const char  *domain,
                                  int          expired,
                                  int          max_age,
                                  guava_bool_t secure,
                                  guava_bool_t httponly) {
  Cookie *cookie = (Cookie *)PyObject_New(Cookie, &CookieType);
  memset(&cookie->data, 0, sizeof(cookie->data));

  if (name) {
    guava_cookie_set_name(&cookie->data, name);
  }
  if (value) {
    guava_cookie_set_value(&cookie->data, value);
  }
  if (path) {
    guava_cookie_set_path(&cookie->data, path);
  }
  if (domain) {
    guava_cookie_set_domain(&cookie->data, domain);
  }
  guava_cookie_set_secure(&cookie->data, secure);
  guava_cookie_set_httponly(&cookie->data, httponly);
  guava_cookie_set_expired(&cookie->data, expired);
  guava_cookie_set_max_age(&cookie->data, max_age);

  return (PyObject *)cookie;
}

guava_cookie_t *guava_cookie_new() {
  guava_cookie_t *cookie = (guava_cookie_t *)guava_malloc(sizeof(*cookie));
  if (!cookie) {
    return NULL;
  }

  cookie->name = NULL;
  cookie->value = NULL;
  cookie->domain = NULL;
  cookie->path = NULL;
  cookie->expired = 0;
  cookie->max_age = 0;
  cookie->secure = GUAVA_FALSE;
  cookie->httponly = GUAVA_FALSE;

  return cookie;
}

void guava_cookie_free(guava_cookie_t *cookie) {
  if (cookie->name) {
    guava_string_free(cookie->name);
  }

  if (cookie->value) {
    guava_string_free(cookie->value);
  }

  if (cookie->domain) {
    guava_string_free(cookie->domain);
  }

  if (cookie->path) {
    guava_string_free(cookie->path);
  }

  guava_free(cookie);
}

void guava_cookie_set_name(guava_cookie_t *cookie,
                           const char     *name) {
  if (cookie->name) {
    guava_string_free(cookie->name);
  }

  cookie->name = guava_string_new(name);
}

guava_string_t guava_cookie_get_name(guava_cookie_t *cookie) {
  return cookie->name;
}

void guava_cookie_set_value(guava_cookie_t *cookie,
                            const char     *value) {
  if (cookie->value) {
    guava_string_free(cookie->value);
  }

  cookie->value = guava_string_new(value);
}

guava_string_t guava_cookie_get_value(guava_cookie_t *cookie) {
  return cookie->value;
}

void guava_cookie_set_domain(guava_cookie_t *cookie,
                             const char     *domain) {
  if (cookie->domain) {
    guava_string_free(cookie->domain);
  }

  cookie->domain = guava_string_new(domain);
}

guava_string_t guava_cookie_get_domain(guava_cookie_t *cookie) {
  return cookie->domain;
}

void guava_cookie_set_path(guava_cookie_t *cookie,
                           const char     *path) {
  if (cookie->path) {
    guava_string_free(cookie->path);
  }

  cookie->path = guava_string_new(path);
}

guava_string_t guava_cookie_get_path(guava_cookie_t *cookie) {
  return cookie->path;
}

void guava_cookie_set_expired(guava_cookie_t *cookie,
                              time_t          expired) {
  cookie->expired = expired;
}

time_t guava_cookie_get_expired(guava_cookie_t *cookie) {
  return cookie->expired;
}

void guava_cookie_set_max_age(guava_cookie_t *cookie,
                              time_t          max_age) {
  cookie->max_age = max_age;
}

time_t guava_cookie_get_max_age(guava_cookie_t *cookie) {
  return cookie->max_age;
}

void guava_cookie_set_secure(guava_cookie_t *cookie,
                             guava_bool_t    secure) {
  cookie->secure = secure;
}

guava_bool_t guava_cookie_get_secure(guava_cookie_t *cookie) {
  return cookie->secure;
}

void guava_cookie_set_httponly(guava_cookie_t *cookie,
                               guava_bool_t    httponly) {
  cookie->httponly = httponly;
}

guava_bool_t guava_cookie_get_httponly(guava_cookie_t *cookie) {
  return cookie->httponly;
}

PyObject *guava_cookie_parse(char **data) {
  if (!data || *data == '\0') {
    return NULL;
  }

  guava_string_t name = NULL;
  guava_string_t value = NULL;

  char *p = *data;

  char *name_start = NULL;
  char *name_end = NULL;
  char *value_start = NULL;
  char *value_end = NULL;
  Cookie *cookie = NULL;

  while(isspace(*p)) {
    ++p;
  }

  do {
    if (*p == '\0') {
      break;
    }

    name_start = p;

    while (*p != '=') {
      ++p;
    }

    if (*p != '=') {
      break;
    }

    name_end = p;

    p += 1;

    if (*p == '\0') {
      break;
    }

    value_start = p;

    while (*p != ';' && *p != '\0') {
      ++p;
    }

    value_end = p;

    if (*p == ';') {
      ++p;
    }

    *data = p;
  } while(0);

  if (!name_start || !name_end || !value_start || !value_end) {
    return NULL;
  }

  name = guava_string_new_size(name_start, name_end - name_start);
  value = guava_string_new_size(value_start, value_end - value_start);

  cookie = PyObject_New(Cookie, &CookieType);
  guava_cookie_set_name(&cookie->data, name);
  guava_cookie_set_value(&cookie->data, value);

  guava_string_free(name);
  guava_string_free(value);

  return (PyObject *)cookie;
}
