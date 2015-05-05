# Copyright 2015 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
from guava.cookie import Cookie


class TestCookie(unittest.TestCase):

    def test_cookie(self):
        c = Cookie(name='name', value='rock')
        self.assertEqual(c.name, 'name')
        self.assertEqual(c.value, 'rock')

        c.name = 'name2'
        self.assertEqual(c.name, 'name2')

        c.value = 'rock2'
        self.assertEqual(c.value, 'rock2')


if __name__ == '__main__':
    unittest.main()
