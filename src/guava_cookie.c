/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_cookie.h"
#include "guava_string.h"
#include "guava_memory.h"

guava_cookie_t *
guava_cookie_new(const char   *name,
                 const char   *value,
                 const char   *path,
                 const char   *domain,
                 int           expired,
                 int           max_age,
                 guava_bool_t  secure,
                 guava_bool_t  httponly)
{
  guava_cookie_t *cookie = (guava_cookie_t *)guava_calloc(1, sizeof(*cookie));
  if (!cookie) {
    return NULL;
  }

  do {
    if (!guava_cookie_set_name(cookie, name)) break;
    if (!guava_cookie_set_value(cookie, value)) break;
    if (!guava_cookie_set_path(cookie, path)) break;
    if (!guava_cookie_set_domain(cookie, domain)) break;
    if (!guava_cookie_set_expired(cookie, expired)) break;
    if (!guava_cookie_set_max_age(cookie, max_age)) break;
    if (!guava_cookie_set_secure(cookie, secure)) break;
    if (!guava_cookie_set_httponly(cookie, httponly)) break;

    return cookie;
  } while(0);

  guava_free(cookie);
  return NULL;
}

void
guava_cookie_destroy(guava_cookie_t *cookie)
{
  if (cookie->name) guava_string_free(cookie->name);
  if (cookie->value) guava_string_free(cookie->value);
  if (cookie->path) guava_string_free(cookie->path);
  if (cookie->domain) guava_string_free(cookie->domain);

  guava_free(cookie);
}

guava_bool_t
guava_cookie_set_name(guava_cookie_t *cookie,
                      const char     *name)
{
  if (cookie->name) guava_string_free(cookie->name);

  cookie->name = guava_string_new(name);
  return GUAVA_TRUE;
}

guava_bool_t
guava_cookie_set_value(guava_cookie_t *cookie,
                       const char     *value)
{
  if (cookie->value) guava_string_free(cookie->value);

  cookie->value = guava_string_new(value);
  return GUAVA_TRUE;
}

guava_bool_t
guava_cookie_set_path(guava_cookie_t *cookie,
                      const char     *path)
{
  if (cookie->path) guava_string_free(cookie->path);

  cookie->path = guava_string_new(path);
  return GUAVA_TRUE;
}

guava_bool_t
guava_cookie_set_domain(guava_cookie_t *cookie,
                        const char     *domain)
{
  if (cookie->domain) guava_string_free(cookie->domain);

  cookie->domain = guava_string_new(domain);
  return GUAVA_TRUE;
}

guava_bool_t
guava_cookie_set_expired(guava_cookie_t *cookie,
                         int             expired)
{
  cookie->expired = expired;
  return GUAVA_TRUE;
}

guava_bool_t
guava_cookie_set_max_age(guava_cookie_t *cookie,
                         int             max_age)
{
  cookie->max_age = (int)max_age;
  return GUAVA_TRUE;
}

guava_bool_t
guava_cookie_set_secure(guava_cookie_t *cookie,
                        guava_bool_t    secure)
{
  cookie->secure = secure;
  return GUAVA_TRUE;
}

guava_bool_t
guava_cookie_set_httponly(guava_cookie_t *cookie,
                          guava_bool_t    httponly)
{
  cookie->httponly = httponly;
  return GUAVA_TRUE;
}

/* PyObject *guava_cookie_parse(char **data) { */
/*   if (!data || *data == '\0') { */
/*     return NULL; */
/*   } */

/*   guava_string_t name = NULL; */
/*   guava_string_t value = NULL; */

/*   char *p = *data; */

/*   char *name_start = NULL; */
/*   char *name_end = NULL; */
/*   char *value_start = NULL; */
/*   char *value_end = NULL; */
/*   Cookie *cookie = NULL; */

/*   while(isspace(*p)) { */
/*     ++p; */
/*   } */

/*   do { */
/*     if (*p == '\0') { */
/*       break; */
/*     } */

/*     name_start = p; */

/*     while (*p != '=') { */
/*       ++p; */
/*     } */

/*     if (*p != '=') { */
/*       break; */
/*     } */

/*     name_end = p++; */

/*     if (*p == '\0') { */
/*       break; */
/*     } */

/*     value_start = p; */

/*     while (*p != ';' && *p != '\0') { */
/*       ++p; */
/*     } */

/*     value_end = p; */

/*     if (*p == ';') { */
/*       ++p; */
/*     } */

/*     *data = p; */
/*   } while(0); */

/*   if (!name_start || !name_end || !value_start || !value_end) { */
/*     return NULL; */
/*   } */

/*   name = guava_string_new_size(name_start, name_end - name_start); */
/*   value = guava_string_new_size(value_start, value_end - value_start); */

/*   cookie = PyObject_New(Cookie, &CookieType); */
/*   memset(&cookie->data, 0, sizeof(guava_cookie_t)); */
/*   guava_cookie_set_name(&cookie->data, name); */
/*   guava_cookie_set_value(&cookie->data, value); */

/*   guava_string_free(name); */
/*   guava_string_free(value); */

/*   return (PyObject *)cookie; */
/* } */
