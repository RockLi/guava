# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
import guava

class TestHandler(unittest.TestCase):
    def setUp(self):
        pass

    def test_handler(self):
        handler = guava.handler.Handler(module='module1',
                                        cls='Class',
                                        action='action')

        self.assertTrue(handler.package, '.')
        self.assertTrue(handler.module, 'module1')
        self.assertTrue(handler.cls, 'Class')
        self.assertTrue(handler.action, 'action')

    def test_handler_init_package(self):
        handler = guava.handler.Handler(module='module1',
                                        cls='Class',
                                        action='action',
                                        package='p1')

        self.assertTrue(handler.package, 'p1')

    def test_handler_getattr(self):
        handler = guava.handler.Handler(module='module1',
                                        cls='Class',
                                        action='action',
                                        package='p1')

        handler.package = 'p2'
        self.assertTrue(handler.package, 'p2')

        handler.module = 'module2'
        self.assertTrue(handler.module, 'module2')

        handler.cls = 'Class2'
        self.assertTrue(handler.cls, 'Class2')

        handler.action = 'action2'
        self.assertTrue(handler.action, 'action2')


if __name__ == '__main__':
	unittest.main()
