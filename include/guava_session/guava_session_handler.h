/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_SESSION_HANDLER_H__
#define __GUAVA_SESSION_HANDLER_H__

#include "guava_hashtable.h"
#include "guava_string.h"

/**
 * generate a new session id
 */
typedef guava_string_t (*guava_session_handler_create_session_id)();

/**
 * read the serialized data of specific session id
 */
typedef guava_string_t (*guava_session_handler_read)(const char *session_id);

/**
 * write the serialized data of specific session id
 */
typedef guava_bool_t (*guava_session_handler_write)(const char *session_id, const char *data);

/**
 * release the resource of specific session id
 */
typedef guava_string_t (*guava_session_handler_destroy)(const char *session_id);

/**
 * Default handler for generate a new session id
 */
guava_string_t guava_session_handler_create_session_id_default();

#endif /* !__GUAVA_SESSION_HANDLER_H__ */
