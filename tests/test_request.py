# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest

import guava


class TestRequestInit(unittest.TestCase):

    def setUp(self):
        self.req = guava.request.Request(url='/about',
                                         path='/about',
                                         host='www.google.com',
                                         method='POST',
                                         body='Hello World',
                                         HEADERS={'UserAgent': 'xxx'},
                                         GET={'a': 10},
                                         POST={'a':20})

        self.req2 = guava.request.Request(url='/about',
                                         host='www.google.com',
                                         method='POST',
                                         body='Hello World',
                                         HEADERS={'UserAgent': 'xxx'},
                                         GET={'a': 10},
                                         POST={'a':20})


    def test_url(self):
        self.assertEqual(self.req.url, '/about')
        self.assertEqual(self.req2.url, '/about')

    def test_path(self):
        self.assertEqual(self.req.path, '/about')
        self.assertEqual(self.req2.path, '/about')

    def test_host(self):
        self.assertEqual(self.req.host, 'www.google.com')

    def test_method(self):
        self.assertEqual(self.req.method, 'POST')

    def test_body(self):
        self.assertEqual(self.req.body, 'Hello World')

    def test_HEADERS(self):
        self.assertEqual(self.req.HEADERS, {'UserAgent': 'xxx'})

    def test_GET(self):
        self.assertEqual(self.req.GET, {'a': 10})

    def test_POST(self):
        self.assertEqual(self.req.POST, {'a': 20})


if __name__ == '__main__':
    unittest.main()
