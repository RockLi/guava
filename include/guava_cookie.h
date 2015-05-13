/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_COOKIE_H__
#define __GUAVA_COOKIE_H__

#include "guava_config.h"
#include "guava_string.h"

typedef struct guava_cookie {
  guava_string_t  name;
  guava_string_t  value;
  guava_string_t  path;
  guava_string_t  domain;
  int             expired;
  int             max_age;
  guava_bool_t    secure;
  guava_bool_t    httponly;
} guava_cookie_t;

/**
 * Create a new cookie object
 */
guava_cookie_t *
guava_cookie_new(const char    *name,
                 const char    *value,
                 const char    *path,
                 const char    *domain,
                 int            expired,
                 int            max_age,
                 guava_bool_t   secure,
                 guava_bool_t   httponly);

/**
 * Destroy a cookie object
 */
void
guava_cookie_destroy(guava_cookie_t *cookie);


/**
 * Set the name of the cookie
 */
guava_bool_t
guava_cookie_set_name(guava_cookie_t *cookie,
                      const char     *name);

/**
 * Set the value of the cookie
 */
guava_bool_t
guava_cookie_set_value(guava_cookie_t *cookie,
                       const char     *value);

/**
 * Set the domain of the cookie
 */
guava_bool_t
guava_cookie_set_domain(guava_cookie_t *cookie,
                        const char     *domain);

/**
 * Set the path of the cookie
 */
guava_bool_t
guava_cookie_set_path(guava_cookie_t *cookie,
                      const char     *path);

/**
 * Set the expired attribute of the cookie
 */
guava_bool_t
guava_cookie_set_expired(guava_cookie_t *cookie,
                         int             expired);

/**
 * Set the max-age attribute of the cookie
 */
guava_bool_t
guava_cookie_set_max_age(guava_cookie_t *cookie,
                         int            max_age);

/**
 * Enable the secure of the cookie
 */
guava_bool_t
guava_cookie_set_secure(guava_cookie_t *cookie,
                        guava_bool_t    secure);

/**
 * Enable/Disable the httponly of the cookie
 */
guava_bool_t
guava_cookie_set_httponly(guava_cookie_t *cookie,
                          guava_bool_t    httponly);

/* PyObject *guava_cookie_parse(char **data); */

#endif /* !__GUAVA_COOKIE_H__ */
