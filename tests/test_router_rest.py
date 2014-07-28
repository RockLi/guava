# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
import guava


class TestRouterREST(unittest.TestCase):

    def setUp(self):
        self.router = guava.router.RESTRouter(mount_point="/")

    def test_mount_point_getset(self):
        self.assertEqual(self.router.mount_point, '/')

        self.router.mount_point = '/about'
        self.assertEqual(self.router.mount_point, '/about/')
        self.router.mount_point = '/'

    def test_route(self):
        req = guava.request.Request(url='/users/', method='GET')
        self.assert_handler(self.router.route(req), '.', 'users', 'UsersController', 'get_all')

        req = guava.request.Request(url='/users/10', method='GET')
        self.assert_handler(self.router.route(req), '.', 'users', 'UsersController', 'get_one')

        req = guava.request.Request(url='/users/10', method='DELETE')
        self.assert_handler(self.router.route(req), '.', 'users', 'UsersController', 'delete_one')

        req = guava.request.Request(url='/users/', method='POST')
        self.assert_handler(self.router.route(req), '.', 'users', 'UsersController', 'create_one')

        req = guava.request.Request(url='/users/10', method='PUT')
        self.assert_handler(self.router.route(req), '.', 'users', 'UsersController', 'update_one')

    def assert_handler(self, handler, package, module, cls, action, args=()):
        self.assertEqual(handler.package, package)
        self.assertEqual(handler.module, module)
        self.assertEqual(handler.cls, cls)
        self.assertEqual(handler.action, action)

        if args:
            self.assertEqual(handler.args, args)


if __name__ == '__main__':
    unittest.main()
