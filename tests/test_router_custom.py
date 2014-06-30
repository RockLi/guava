# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
import guava

class CustomRouter(guava.router.Router):

    def __int__(self, *args, **kwargs):
        super(CustomRouter, self).__init__(*args, **kwargs)

    def route(self, req):
        if req.url == '/about':
            return guava.handler.Handler(package='.',
                                         module='about',
                                         cls='AboutController',
                                         action='index')

        return None

class TestCustomRouter(unittest.TestCase):

    def setUp(self):
        self.router = CustomRouter({
            '/me': guava.handler.Handler(package='.',
                                         module='me',
                                         cls='MeController',
                                         action='index')
        })

    def test_routes(self):
        routes = self.router.routes()
        handler = routes.get('/me', None)

        self.assertNotEqual(handler, None)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'me')
        self.assertEqual(handler.cls, 'MeController')
        self.assertEqual(handler.action, 'index')

    def test_route(self):
        req = guava.request.Request()
        req.url = '/me'
        req.method = 'GET'

        handler = self.router.route(req)
        self.assertEqual(handler, None)

        req.url = '/about'

        handler = self.router.route(req)
        self.assertNotEqual(handler, None)
        self.assertEqual(handler.package, '.')
        self.assertEqual(handler.module, 'about')
        self.assertEqual(handler.cls, 'AboutController')
        self.assertEqual(handler.action, 'index')


if __name__ == '__main__':
    unittest.main()
