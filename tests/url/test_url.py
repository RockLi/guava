# Copyright 2015 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
from guava.url import URL


class TestURL(unittest.TestCase):

    def setUp(self):
        pass

    def test_default_values(self):
        u = URL()
        self.assertTrue(u.schema == 'http')
        self.assertTrue(u.username is None)
        self.assertTrue(u.password is None)
        self.assertTrue(u.host == 'localhost')
        self.assertTrue(u.port == 80)
        self.assertTrue(u.path == '/')
        self.assertTrue(u.queries is None)
        self.assertTrue(u.fragment is None)

    def test_schema(self):
        u = URL(schema='https')
        self.assertEqual(u.schema, 'https')
        u.schema = 'http'
        self.assertEqual(u.schema, 'http')

    def test_username(self):
        u = URL()
        self.assertEqual(u.username, None)
        u.username = 'rock'
        self.assertEqual(u.username, 'rock')

    def test_password(self):
        u = URL()
        self.assertEqual(u.password, None)
        u.password = '123456'
        self.assertEqual(u.password, '123456')

    def test_host(self):
        u = URL()
        self.assertEqual(u.host, 'localhost')
        u.host = 'www.google.com'
        self.assertEqual(u.host, 'www.google.com')

    def test_port(self):
        u = URL()
        self.assertEqual(u.port, 80)
        u.port = 30000
        self.assertEqual(u.port, 30000)

    def test_path(self):
        u = URL()
        self.assertEqual(u.path, '/')
        u.path = '/helloworld'
        self.assertEqual(u.path, '/helloworld')

    def test_queries(self):
        u = URL(queries={'name': 'rock'})
        self.assertEqual(u.queries, {'name': 'rock'})
        u.set_query('name', 'rock-rock')
        self.assertEqual(u.queries, {'name': 'rock-rock'})
        self.assertEqual(u.get_query('name'), 'rock-rock')

    def test_fragment(self):
        u = URL()
        self.assertEqual(u.fragment, None)
        u.fragment = None
        self.assertEqual(u.fragment, None)
        u.fragment = 'id1'
        self.assertEqual(u.fragment, 'id1')

    def test_unencoded(self):
        u = URL(host='www.google.com', port=8080, queries={
            'name': 'rock',
            'age': 27,
        })

        self.assertEqual(u.encoded(), 'http://www.google.com:8080/?name=rock&age=27')


if __name__ == '__main__':
    unittest.main()
