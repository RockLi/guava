# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
import guava


class TestRouter(unittest.TestCase):

    def setUp(self):
        self.router = guava.router.Router()
        self.about_handler = guava.handler.Handler(package='.',
                                                   module='about',
                                                   cls='AboutController',
                                                   action='index')

        self.me_handler = guava.handler.Handler(package='.',
                                                module='me',
                                                cls='MeController',
                                                action='index')

        self.router.register('/about', self.about_handler)
        self.router.register('/me', self.me_handler)

    def test_register(self):
        self.assertTrue(self.router.routes(), {
            '/about': self.about_handler,
            '/me': self.me_handler,
        })

    def test_route(self):
        req = guava.request.Request()
        req.url = '/about'
        req.method = 'GET'
        self.assertEqual(self.router.route(req), self.about_handler)

        req.url = '/me'
        self.assertEqual(self.router.route(req), self.me_handler)

        req.url = '/404'
        self.assertEqual(self.router.route(req), None)
        req.url = '/about_new'
        self.assertEqual(self.router.route(req), None)

    def test_router_init_with_routes(self):
        about_handler = guava.handler.Handler(package='.',
                                              module='about',
                                              cls='AboutController',
                                              action='index')
        router = guava.router.Router({
            '/about': about_handler,
        })

        self.assertEqual(router.routes(), {
            '/about': about_handler,
        })


if __name__ == '__main__':
    unittest.main()
