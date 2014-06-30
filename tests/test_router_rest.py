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
        req = guava.request.Request(url='/',
                                    method='GET')

        req.url = '/users/'
        handler = self.router.route(req)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'users')
        self.assertEqual(handler.cls, 'UsersController')
        self.assertEqual(handler.action, 'get_all')

        req.url = '/users/10'
        handler = self.router.route(req)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'users')
        self.assertEqual(handler.cls, 'UsersController')
        self.assertEqual(handler.action, 'get_one')

        req.url = '/users/10'
        req.method = 'DELETE'
        handler = self.router.route(req)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'users')
        self.assertEqual(handler.cls, 'UsersController')
        self.assertEqual(handler.action, 'delete_one')

        req.url = '/users/'
        req.method = 'POST'
        handler = self.router.route(req)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'users')
        self.assertEqual(handler.cls, 'UsersController')
        self.assertEqual(handler.action, 'create_one')


        req.url = '/users/10'
        req.method = 'PUT'
        handler = self.router.route(req)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'users')
        self.assertEqual(handler.cls, 'UsersController')
        self.assertEqual(handler.action, 'update_one')


if __name__ == '__main__':
    unittest.main()
