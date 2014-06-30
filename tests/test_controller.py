# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
import guava


class MyController(guava.controller.Controller):

    def index(self):
        pass


class TestController(unittest.TestCase):

    def setUp(self):
        pass

    def test_controller(self):
        c = MyController()

        self.assertTrue(c.header != None)
        self.assertTrue(c.write != None)
        self.assertTrue(c.status_code != None)
        self.assertTrue(c.redirect != None)
        self.assertTrue(c.hook_before_action != None)
        self.assertTrue(c.hook_after_action != None)
        self.assertTrue(c.cookie != None)


if __name__ == '__main__':
	unittest.main()
