/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_url.h"
#include "guava_string.h"
#include "guava_hashtable.h"
#include "guava_memory.h"
#include "guava_util.h"

static char hex_codes[] = "0123456789ABCDEF";

char n_from_hex(uint8_t c) {
  return isdigit(c) ? c - 0x30 : toupper(c) - 0x41 + 0x0A;
}

char n_to_hex(uint8_t c) {

  assert(c >= 0 && c <= 0x0F);
  return hex_codes[c & 0x0F];
}

guava_string_t guava_url_encode(const char *str) {
  char *pstr = (char *)str, *buf = guava_malloc(strlen(str) * 3 + 1), *pbuf = buf;
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
      *pbuf++ = *pstr;
    } else if (*pstr == ' ') {
      *pbuf++ = '+';
    } else {
      *pbuf++ = '%', *pbuf++ = n_to_hex(*pstr >> 4), *pbuf++ = n_to_hex(*pstr & 15);
    }
    pstr++;
  }
  *pbuf = '\0';
  guava_string_t ret = guava_string_new(buf);
  guava_free(buf);
  return ret;
}

guava_string_t guava_url_decode(const char *str) {
  char *pstr = (char *)str, *buf = guava_malloc(strlen(str) + 1), *pbuf = buf;
  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        *pbuf++ = n_from_hex(pstr[1]) << 4 | n_from_hex(pstr[2]);
        pstr += 2;
      }
    } else if (*pstr == '+') {
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  guava_string_t ret = guava_string_new(buf);
  guava_free(buf);
  return ret;
}


guava_url_t *
guava_url_new(const char        *schema,
              const char        *username,
              const char        *password,
              const char        *host,
              uint16_t           port,
              const char        *path,
              guava_hashtable_t *queries,
              const char        *fragment)
{
  guava_url_t *url = (guava_url_t *)guava_calloc(1, sizeof(guava_url_t));
  if (!url) return NULL;

  do {
    if(!guava_url_set_schema(url, schema)) break;
    if (!guava_url_set_username(url, username)) break;
    if (!guava_url_set_password(url, password)) break;
    if (!guava_url_set_host(url, host)) break;
    if (!guava_url_set_port(url, port)) break;
    if (!guava_url_set_path(url, path)) break;
    if (!guava_url_set_queries(url, queries)) break;
    if (!guava_url_set_fragment(url, fragment)) break;

    return url;
  } while(0);

  guava_free(url);
  return NULL;
}

void
guava_url_destroy(guava_url_t *url)
{
  if (!url) return;

  if (url->schema) {
    guava_string_free(url->schema);
    url->schema = NULL;
  }

  if (url->username) {
    guava_string_free(url->username);
    url->username = NULL;
  }

  if (url->password) {
    guava_string_free(url->password);
    url->password = NULL;
  }

  if (url->host) {
    guava_string_free(url->host);
    url->host = NULL;
  }

  if (url->path) {
    guava_string_free(url->path);
    url->path = NULL;
  }

  if (url->queries) {
    guava_hashtable_destroy(url->queries);
    url->queries = NULL;
  }

  if (url->fragment) {
    guava_string_free(url->fragment);
    url->fragment = NULL;
  }

  guava_free(url);
}

guava_bool_t
guava_url_set_schema(guava_url_t *url,
                     const char  *schema)
{
  if ((strncasecmp(schema, "http", strlen("http")) != 0) &&
      strncasecmp(schema, "https", strlen("https")) != 0) {
    /* Right now, we only support http(s) */
    return GUAVA_FALSE;
  }

  if (url->schema) {
    guava_string_free(url->schema);
  }

  url->schema = schema ? guava_string_new(schema) : NULL;
  guava_lowercase((char *)url->schema, guava_string_len(url->schema));
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_set_username(guava_url_t *url,
                       const char  *username)
{
  if (url->username) {
    guava_string_free(url->username);
  }

  url->username = username ? guava_string_new(username) : NULL;
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_set_password(guava_url_t *url,
                       const char  *password)
{
  if (url->password) {
    guava_string_free(url->password);
  }

  url->password = password ? guava_string_new(password) : NULL;
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_set_host(guava_url_t *url,
                   const char  *host)
{
  if (url->host) {
    guava_string_free(url->host);
  }

  url->host = host ? guava_string_new(host) : NULL;
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_set_port(guava_url_t *url,
                   uint16_t     port)
{
  url->port = port;
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_set_path(guava_url_t *url,
                   const char  *path)
{
  if (url->path) {
    guava_string_free(url->path);
  }

  url->path = path ? guava_string_new(path) : NULL;
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_set_query(guava_url_t *url,
                    const char  *name,
                    const char  *value)
{
  if (!url->queries){
    url->queries = guava_hashtable_new(4,
                                       guava_hashtable_key_hash_func_default,
                                       guava_hashtable_key_cmp_func_default,
                                       guava_string_free,
                                       guava_string_free,
                                       NULL);
    if (!url->queries){
      return GUAVA_FALSE;
    }
  }

  guava_string_t v = (guava_string_t)guava_hashtable_find(url->queries,
                                                          name);
  if (v) {
    guava_hashtable_update(url->queries,
                           guava_string_new(name),
                           guava_string_new(value));
  } else {
    guava_hashtable_insert(url->queries,
                           guava_string_new(name),
                           guava_string_new(value));
  }

  return GUAVA_TRUE;
}

guava_string_t
guava_url_get_query(guava_url_t *url,
                    const char  *name)
{
  if (!url->queries){
    return NULL;
  }

  guava_string_t v = (guava_string_t)guava_hashtable_find(url->queries,
                                                          name);
  return v;
}

guava_bool_t
guava_url_remove_query(guava_url_t *url,
                       const char  *name)
{
  if (!url->queries){
    return GUAVA_TRUE;
  }

  guava_hashtable_delete(url->queries, name);
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_set_queries(guava_url_t       *url,
                      guava_hashtable_t *queries)
{
  if (url->queries) {
    guava_hashtable_destroy(url->queries);
  }

  url->queries = queries;
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_remove_queries(guava_url_t *url)
{
  if (!url->queries) {
    return GUAVA_TRUE;
  }

  guava_hashtable_destroy(url->queries);
  return GUAVA_TRUE;
}

guava_bool_t
guava_url_set_fragment(guava_url_t *url,
                       const char  *fragment)
{
  if (url->fragment) {
    guava_string_free(url->fragment);
  }

  url->fragment = fragment ? guava_string_new(fragment) : NULL;
  return GUAVA_TRUE;
}

guava_url_t *
guava_url_parse(const char *raw)
{
  /* @Todo */
  return NULL;
}

guava_string_t
guava_url_format(guava_url_t *url, guava_bool_t encode)
{
  if (!url) {
    return NULL;
  }
  guava_string_t ret = NULL;
  ret = guava_string_append(ret, url->schema);
  ret = guava_string_append_raw(ret, "://");

  if (url->username && url->password) {
    ret = guava_string_append(ret, url->username);
    ret = guava_string_append_raw(ret, ":");
    ret = guava_string_append(ret, url->password);
    ret = guava_string_append_raw(ret, "@");
  }

  ret = guava_string_append(ret, url->host);
  if (url->port != 80) {
    ret = guava_string_append_raw(ret, ":");
    char buf[8] = {0};
    snprintf(buf, sizeof(buf), "%d", url->port);
    ret = guava_string_append_raw(ret, buf);
  }

  if (url->path[0] != '/') {
    ret = guava_string_append_raw(ret, "/");
  }
  ret = guava_string_append(ret, url->path);

  if (url->queries){
    ret = guava_string_append_raw(ret, "?");
    guava_hashtable_iter_t *it = guava_hashtable_iter_new(url->queries);
    size_t i = 0;
    while (guava_hashtable_iter_valid(it)) {
      const char *key = guava_hashtable_iter_get_key(it);
      guava_string_t value = (guava_string_t)guava_hashtable_iter_get_value(it);
      if (i++ > 0) {
        ret = guava_string_append_raw(ret, "&");
      }
      if (encode) {
        guava_string_t encoded = guava_url_encode(key);
        ret = guava_string_append(ret, encoded);
        guava_string_free(encoded);
      } else {
        ret = guava_string_append_raw(ret, key);
      }
      ret = guava_string_append_raw(ret, "=");
      if (encode) {
        guava_string_t encoded = guava_url_encode(value);
        ret = guava_string_append(ret, encoded);
        guava_string_free(encoded);
      } else {
        ret = guava_string_append(ret, value);
      }

      if (guava_hashtable_iter_next(it) == GUAVA_FALSE)
        break;
    }
    guava_hashtable_iter_destroy(it);

  }

  if (url->fragment) {
    ret = guava_string_append_raw(ret, "#");
    ret = guava_string_append(ret, url->fragment);
  }

  return ret;
}

guava_string_t
guava_url_encoded(guava_url_t *url)
{
  return guava_url_format(url, GUAVA_TRUE);
}

guava_string_t
guava_url_unencoded(guava_url_t *url)
{
  return guava_url_format(url, GUAVA_FALSE);
}


#if 1

int
main(int argc, char **argv){
  guava_url_t *url = guava_url_new("http", NULL, NULL, "localhost", 5000, "/", NULL, "id1");
  guava_url_set_query(url, "name", "rock");
  guava_url_set_query(url, "D%A", "27");

  guava_string_t s = guava_url_unencoded(url);
  printf("%s\n", s);

  s = guava_url_encoded(url);
  printf("%s\n", s);

  return 0;
}

#endif
