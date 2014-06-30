/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_CONN_H__
#define __GUAVA_CONN_H__

#include "guava.h"

guava_conn_t *guava_conn_new(void);

void guava_conn_free(guava_conn_t *conn);

#endif /* !__GUAVA_CONN_H__ */
