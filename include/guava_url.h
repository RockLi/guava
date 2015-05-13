/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_URL_H__
#define __GUAVA_URL_H__

#include "guava_config.h"
#include "guava_string.h"
#include "guava_hashtable.h"

typedef struct guava_url {
  guava_string_t     schema;
  guava_string_t     username;
  guava_string_t     password;
  guava_string_t     host;
  uint16_t           port;
  guava_string_t     path;
  guava_hashtable_t *queries;
  guava_string_t     fragment;
} guava_url_t;


/**
 * Create a new URL representation
 */
guava_url_t *
guava_url_new(const char        *schema,
              const char        *username,
              const char        *password,
              const char        *host,
              uint16_t           port,
              const char        *path,
              guava_hashtable_t *queries,
              const char        *fragment);

/**
 * Create a new URL from raw string
 */
guava_url_t *
guava_url_parse(const char *raw);

/**
 * Release the resource of a URL struct
 */
void
guava_url_destroy(guava_url_t *url);

guava_bool_t
guava_url_set_schema(guava_url_t *url,
                     const char  *schema);

guava_bool_t
guava_url_set_username(guava_url_t *url,
                       const char  *username);

guava_bool_t
guava_url_set_password(guava_url_t *url,
                       const char  *password);

guava_bool_t
guava_url_set_host(guava_url_t *url,
                   const char  *host);

guava_bool_t
guava_url_set_port(guava_url_t *url,
                   uint16_t     port);

guava_bool_t
guava_url_set_path(guava_url_t *url,
                   const char  *path);

guava_bool_t
guava_url_set_query(guava_url_t *url,
                    const char  *name,
                    const char  *value);

guava_string_t
guava_url_get_query(guava_url_t *url,
                    const char  *name);

guava_bool_t
guava_url_remove_query(guava_url_t *url,
                       const char  *name);

guava_bool_t
guava_url_set_queries(guava_url_t       *url,
                      guava_hashtable_t *queries);

guava_bool_t
guava_url_remove_queries(guava_url_t *url);

guava_bool_t
guava_url_set_fragment(guava_url_t *url,
                       const char  *fragment);

/**
 * Encoded representation of the URL
 */
guava_string_t
guava_url_encoded(guava_url_t *url);

/**
 * Unencoded format
 * For example: http://localhost:8080/?name=rock&age=27
 */
guava_string_t
guava_url_unencoded(guava_url_t *url);

#endif /* !__GUAVA_URL_H__ */
