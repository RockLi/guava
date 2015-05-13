/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_H__
#define __GUAVA_H__

#include <Python.h>
#include <structmember.h>

#include "guava_config.h"

#define GUAVA_VERSION "0.2"

#define GUAVA_SERVER_DEFAULT_NAME "guava-server"
#define GUAVA_SERVER_DEFAULT_LISTEN_IP "0.0.0.0"
#define GUAVA_SERVER_DEFAULT_LISTEN_PORT 8000
#define GUAVA_SERVER_DEFAULT_LISTEN_BACKLOG 128

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

#endif /* !__GUAVA_H__ */
