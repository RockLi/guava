/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_MIME_TYPE_H__
#define __GUAVA_MIME_TYPE_H__

#include "guava.h"

#define GUAVA_DEFAULT_MIME_TYPE "text/plain"

const char *guava_mime_type_guess(const char *filename);

#endif /* !__GUAVA_MIME_TYPE_H__ */
