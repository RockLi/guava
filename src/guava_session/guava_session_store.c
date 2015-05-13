/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_session/guava_session_store.h"
#include "guava.h"
#include "guava_memory.h"
#include "guava_string.h"

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
                        guava_session_handler_destroy            destroy_handler)
{
  guava_session_store_t *store = (guava_session_store_t *)guava_calloc(1, sizeof(*store));
  if (!store) return NULL;

  do {
    if (!guava_session_store_set_name(store, name)) break;
    if (!guava_session_store_set_gc_time(store, gc_time)) break;
    if (!guava_session_store_set_cookie_expired(store, cookie_expired)) break;
    if (!guava_session_store_set_cookie_path(store, cookie_path)) break;
    if (!guava_session_store_set_cookie_domain(store, cookie_domain)) break;
    if (!guava_session_store_set_cookie_secure(store, cookie_secure)) break;
    if (!guava_session_store_set_cookie_httponly(store, cookie_httponly)) break;
    if (!guava_session_store_set_handler_create_session_id(store, create_session_id_handler)) break;
    if (!guava_session_store_set_handler_read(store, read_handler)) break;
    if (!guava_session_store_set_handler_write(store, write_handler)) break;
    if (!guava_session_store_set_handler_destroy(store, destroy_handler)) break;

    return store;
  } while(0);

  guava_free(store);
  return NULL;
}

void
guava_session_store_destroy(guava_session_store_t *store)
{
  if (!store) return;
  if (store->name) guava_string_free(store->name);
  if (store->cookie_path) guava_string_free(store->cookie_path);
  if (store->cookie_domain) guava_string_free(store->cookie_domain);

  guava_free(store);
}

guava_bool_t
guava_session_store_set_name(guava_session_store_t *store,
                             const char            *name)
{
  if (!store || !name) return GUAVA_FALSE;
  if (store->name) guava_string_free(store->name);
  store->name = guava_string_new(name);
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_gc_time(guava_session_store_t *store,
                                int                    gc_time)
{
  if (!store || gc_time < 0) return GUAVA_FALSE;
  store->gc_time = gc_time;
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_cookie_expired(guava_session_store_t *store,
                                       int                    cookie_expired)
{
  if (!store) return GUAVA_FALSE;
  store->cookie_expired = cookie_expired;
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_cookie_path(guava_session_store_t *store,
                                    const char            *path)
{
  if (!store) return GUAVA_FALSE;
  if (store->cookie_path) guava_string_free(store->cookie_path);
  store->cookie_path = guava_string_new(path);
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_cookie_domain(guava_session_store_t *store,
                                      const char            *domain)
{
  if (!store) return GUAVA_FALSE;
  if (store->cookie_domain) guava_string_free(store->cookie_domain);
  store->cookie_domain = guava_string_new(domain);
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_cookie_secure(guava_session_store_t *store,
                                      guava_bool_t           secure)
{
  if (!store) return GUAVA_FALSE;
  store->cookie_secure = secure;
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_cookie_httponly(guava_session_store_t *store,
                                        guava_bool_t           httponly)
{
  if (!store) return GUAVA_FALSE;
  store->cookie_httponly = httponly;
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_handler_create_session_id(guava_session_store_t                   *store,
                                                  guava_session_handler_create_session_id  handler)
{
  if (!store) return GUAVA_FALSE;
  store->create_session_id_handler = handler;
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_handler_read(guava_session_store_t      *store,
                                     guava_session_handler_read  handler)
{
  if (!store) return GUAVA_FALSE;
  store->read_handler = handler;
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_handler_write(guava_session_store_t       *store,
                                      guava_session_handler_write  handler)
{
  if (!store) return GUAVA_FALSE;
  store->write_handler = handler;
  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_set_handler_destroy(guava_session_store_t         *store,
                                        guava_session_handler_destroy  handler)
{
  if (!store) return GUAVA_FALSE;
  store->destroy_handler = handler;
  return GUAVA_TRUE;
}

guava_string_t
guava_session_store_read_entry(guava_session_store_t *store,
                               const char            *session_id)
{
  if (!store || !session_id || !store->read_handler) return NULL;
  guava_string_t data = store->read_handler(session_id);
  if (!data) return NULL;

  return data;
}

guava_bool_t
guava_session_store_write_entry(guava_session_store_t *store,
                          const char            *session_id,
                          const char            *data)
{
  if (!store || !session_id || !store->write_handler) return GUAVA_FALSE;
  if (!store->write_handler(session_id, data)) {
    return GUAVA_FALSE;
  }

  return GUAVA_TRUE;
}

guava_bool_t
guava_session_store_destroy_entry(guava_session_store_t *store,
                                  const char            *session_id)
{
  if (!store || !session_id || !store->destroy_handler) return GUAVA_FALSE;
  if (!store->destroy_handler(session_id)) {
    return GUAVA_FALSE;
  }

  return GUAVA_TRUE;
}
