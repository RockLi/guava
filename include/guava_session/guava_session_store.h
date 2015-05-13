/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_SESSION_STORE_H__
#define __GUAVA_SESSION_STORE_H__

#include "guava_session/guava_session_handler.h"

typedef struct guava_session_store{
  guava_string_t                            name;                       /* session name used to talk with the user agent */
  int                                       gc_time;                    /* recycle the session item while exceeds this time in second */
  int                                       cookie_expired;             /* the cookie expired time */
  guava_string_t                            cookie_path;                /* path of the cookie */
  guava_string_t                            cookie_domain;              /* domain of the cookie */
  guava_bool_t                              cookie_secure;              /* enable secure attribute of the cookie */
  guava_bool_t                              cookie_httponly;            /* enable the httponly attribute of the cookie */
  guava_session_handler_create_session_id   create_session_id_handler;  /* handler to generate a new session id */
  guava_session_handler_read                read_handler;               /* read the serialized data */
  guava_session_handler_write               write_handler;              /* write the serialized data */
  guava_session_handler_destroy             destroy_handler;            /* release the resource */
} guava_session_store_t;

/**
 * Create and initialize a new session storage
 */
guava_session_store_t *
guava_session_store_new(guava_string_t                           name,
                        int                                      gc_time,
                        int                                      cookie_expired,
                        guava_string_t                           cookie_path,
                        guava_string_t                           cookie_domain,
                        guava_bool_t                             cookie_secure,
                        guava_bool_t                             cookie_httponly,
                        guava_session_handler_create_session_id  create_session_id_handler,
                        guava_session_handler_read               read_handler,
                        guava_session_handler_write              write_handler,
                        guava_session_handler_destroy            destroy_handler);

/**
 * Release the resource of the session storage
 */
void
guava_session_store_destroy(guava_session_store_t *store);

/**
 * set the name used for session
 */
guava_bool_t
guava_session_store_set_name(guava_session_store_t *store,
                             const char            *name);

/**
 * set the gc time
 */
guava_bool_t
guava_session_store_set_gc_time(guava_session_store_t *store,
                                int                    gc_time);

/**
 * set the expired
 */
guava_bool_t
guava_session_store_set_cookie_expired(guava_session_store_t *store,
                                       int                    cookie_expired);

/**
 * set the cookie path
 */
guava_bool_t
guava_session_store_set_cookie_path(guava_session_store_t *store,
                                    const char            *path);

/**
 * set the cookie domain
 */
guava_bool_t
guava_session_store_set_cookie_domain(guava_session_store_t *store,
                                      const char            *domain);

/**
 * set the secure attribute
 */
guava_bool_t
guava_session_store_set_cookie_secure(guava_session_store_t *store,
                                      guava_bool_t           secure);

/**
 * set the httponly
 */
guava_bool_t
guava_session_store_set_cookie_httponly(guava_session_store_t *store,
                                        guava_bool_t           httponly);

/**
 * set the handler for generating create session id
 */
guava_bool_t
guava_session_store_set_handler_create_session_id(guava_session_store_t                   *store,
                                                  guava_session_handler_create_session_id  handler);

/**
 * set the handler for read session entry
 */
guava_bool_t
guava_session_store_set_handler_read(guava_session_store_t      *store,
                                     guava_session_handler_read  handler);

/**
 * set the handler for write session entry
 */
guava_bool_t
guava_session_store_set_handler_write(guava_session_store_t       *store,
                                      guava_session_handler_write  handler);

/**
 * release the resource
 */
guava_bool_t
guava_session_store_set_handler_destroy(guava_session_store_t         *store,
                                        guava_session_handler_destroy  handler);

/**
 * Read the data of specific session id
 */
guava_string_t
guava_session_store_read_entry(guava_session_store_t *store,
                               const char            *session_id);

/**
 * Write the data for specific session id
 */
guava_bool_t
guava_session_store_write_entry(guava_session_store_t *store,
                                const char            *session_id,
                                const char            *data);


/**
 * Release the data for specific session id
 */
guava_bool_t
guava_session_store_destroy_entry(guava_session_store_t *store,
                                  const char            *session_id);

/**
 * Generate the session id
 */
guava_string_t
guava_session_store_generate_session_id(guava_session_store_t *store);

#endif /* !__GUAVA_SESSION_STORE_H__ */
