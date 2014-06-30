# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
import guava


class TestRouterMVC(unittest.TestCase):

    def setUp(self):
        self.router = guava.router.MVCRouter(mount_point='/')

    def test_mount_point_getset(self):
        self.assertEqual(self.router.mount_point, '/')

        self.router.mount_point = '/about'
        self.assertEqual(self.router.mount_point, '/about/')
        self.router.mount_point = '/'

    def test_router(self):
        req = guava.request.Request()
        req.url = '/'
        req.method = 'GET'

        handler = self.router.route(req)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'index')
        self.assertEqual(handler.cls, 'IndexController')
        self.assertEqual(handler.action, 'index')

        req.url = '/about'
        handler = self.router.route(req)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'about')
        self.assertEqual(handler.cls, 'AboutController')
        self.assertEqual(handler.action, 'index')

        req.url = '/about/hello'
        handler = self.router.route(req)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'about')
        self.assertEqual(handler.cls, 'AboutController')
        self.assertEqual(handler.action, 'hello')


if __name__ == '__main__':
        unittest.main()
