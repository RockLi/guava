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
        req = guava.request.Request(method='GET', url='/')
        self.assert_handler(self.router.route(req), '.', 'index', 'IndexController', 'index')

        req = guava.request.Request(method='GET', url='/about')
        self.assert_handler(self.router.route(req), '.', 'about', 'AboutController', 'index')

        req = guava.request.Request(method='GET', url='/about/hello')
        self.assert_handler(self.router.route(req), '.', 'about', 'AboutController', 'hello')

        req = guava.request.Request(method='GET', url='/post/view/10')
        self.assert_handler(self.router.route(req), '.', 'post', 'PostController', 'view', ('10',))

        req = guava.request.Request(method='GET', url='/post/move/10/20')
        self.assert_handler(self.router.route(req), '.', 'post', 'PostController', 'move', ('10', '20'))

    def assert_handler(self, handler, package, module, cls, action, args=()):
        self.assertEqual(handler.package, package)
        self.assertEqual(handler.module, module)
        self.assertEqual(handler.cls, cls)
        self.assertEqual(handler.action, action)
        if args:
            self.assertEqual(handler.args, args)


if __name__ == '__main__':
        unittest.main()
