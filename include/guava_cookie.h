#ifndef __GUAVA_COOKIE_H__
#define __GUAVA_COOKIE_H__

#include "guava.h"

guava_cookie_t *guava_cookie_new(void);

void guava_cookie_free(guava_cookie_t *cookie);

void guava_cookie_set_name(guava_cookie_t *cookie,
                           const char     *name);

guava_string_t guava_cookie_get_name(guava_cookie_t *cookie);

void guava_cookie_set_value(guava_cookie_t *cookie,
                            const char     *value);

guava_string_t guava_cookie_get_value(guava_cookie_t *cookie);

void guava_cookie_set_domain(guava_cookie_t *cookie,
                             const char     *domain);

guava_string_t guava_cookie_get_domain(guava_cookie_t *cookie);

void guava_cookie_set_path(guava_cookie_t *cookie,
                           const char     *path);

guava_string_t guava_cookie_get_path(guava_cookie_t *cookie);

void guava_cookie_set_expired(guava_cookie_t *cookie,
                              time_t          expired);

time_t guava_cookie_get_expired(guava_cookie_t *cookie);

void guava_cookie_set_max_age(guava_cookie_t *cookie,
                              time_t          max_age);

time_t guava_cookie_get_max_age(guava_cookie_t *cookie);

void guava_cookie_set_secure(guava_cookie_t *cookie,
                             guava_bool_t    secure);

guava_bool_t guava_cookie_get_secure(guava_cookie_t *cookie);

void guava_cookie_set_httponly(guava_cookie_t *cookie,
                               guava_bool_t    httponly);

guava_bool_t guava_cookie_get_httponly(guava_cookie_t *cookie);

PyObject *guava_cookie_parse(char **data);

#endif /* !__GUAVA_COOKIE_H__ */
