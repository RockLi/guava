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
        req = guava.request.Request(url='/about', method='GET')
        self.assertEqual(self.router.route(req), self.about_handler)

        req = guava.request.Request(url='/me', method='GET')
        self.assertEqual(self.router.route(req), self.me_handler)

        req = guava.request.Request(url='/404', method='GET')
        self.assertEqual(self.router.route(req), None)

        req = guava.request.Request(url='/about_new', method='GET')
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

    def test_multiple_mvc_router(self):
        server = guava.server.Server()

        router1 = guava.router.MVCRouter("/", package='controllers')
        router2 = guava.router.MVCRouter("/admin/", package='controllers.admin')
        router3 = guava.router.MVCRouter("/blog/", package='controllers.blog')
        router4 = guava.router.MVCRouter("/admin/blog/", package='controllers.admin.blog')
        router5 = guava.router.MVCRouter("/cms/", package='controllers.cms')
        router6 = guava.router.MVCRouter("/cms/admin/", package='controllers.cms.admin')
        router7 = guava.router.StaticRouter("/static", directory="./static")

        server.add_router(router1, router2, router3,
                          router4, router5, router6,
                          router7)

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/")),
                            'controllers',
                            'index',
                            'IndexController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/index/abc")),
                            'controllers',
                            'index',
                            'IndexController',
                            'abc')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/abc")),
                            'controllers',
                            'abc',
                            'AbcController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/abc/defg")),
                            'controllers',
                            'abc',
                            'AbcController',
                            'defg')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/admin")),
                            'controllers',
                            'admin',
                            'AdminController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/admin/")),
                            'controllers.admin',
                            'index',
                            'IndexController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/blog/abc")),
                            'controllers.blog',
                            'abc',
                            'AbcController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/admin/blog/")),
                            'controllers.admin.blog',
                            'index',
                            'IndexController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/admin/blog/abc")),
                            'controllers.admin.blog',
                            'abc',
                            'AbcController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/admin/blog/abc/")),
                            'controllers.admin.blog',
                            'abc',
                            'AbcController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/admin/blog/abc/defg")),
                            'controllers.admin.blog',
                            'abc',
                            'AbcController',
                            'defg')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/cms/admin/")),
                            'controllers.cms.admin',
                            'index',
                            'IndexController',
                            'index')

        self.assert_handler(server.route(guava.request.Request(method="GET", url="/cms/")),
                            'controllers.cms',
                            'index',
                            'IndexController',
                            'index')

        handler = server.route(guava.request.Request(method="GET", url="/static/1.jpg"))
        self.assertTrue(isinstance(handler, guava.handler.StaticHandler))


    def assert_handler(self, handler, package, module, cls, action, args=()):
        self.assertEqual(handler.package, package)
        self.assertEqual(handler.module, module)
        self.assertEqual(handler.cls, cls)
        self.assertEqual(handler.action, action)
        if args:
            self.assertEqual(handler.args, args)


if __name__ == '__main__':
    unittest.main()
