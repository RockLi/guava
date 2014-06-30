/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef __GUAVA_MODULE_ROUTER_H__
#define __GUAVA_MODULE_ROUTER_H__

#include "guava_module.h"

typedef struct {
  Router router;
} StaticRouter;

typedef struct {
  Router router;
} MVCRouter;

typedef struct {
  Router router;
} RESTRouter;

extern PyTypeObject StaticRouterType;

extern PyTypeObject MVCRouterType;

extern PyTypeObject RESTRouterType;

#endif /* !__GUAVA_MODULE_ROUTER_H__ */
