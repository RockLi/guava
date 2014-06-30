# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest

import guava

class TestRequest(unittest.TestCase):

    def setUp(self):
        self.req = guava.request.Request()

    def tearDown(self):
        self.req = None

    def test_url_getset(self):
        self.req.url = 'url1'
        self.assertEqual(self.req.url, 'url1')

    def test_host_getset(self):
        self.req.host = 'host'
        self.assertEqual(self.req.host, 'host')

    def test_method_getset(self):
        self.req.method = 'GET'
        self.assertEqual(self.req.method, 'GET')

        self.req.method = 'POST'
        self.assertEqual(self.req.method, 'POST')

        self.req.method = 'PUT'
        self.assertEqual(self.req.method, 'PUT')

        self.req.method = 'DELETE'
        self.assertEqual(self.req.method, 'DELETE')

        try:
            self.req.method = 'XX'
            self.assertFalse(True)
        except:
            self.assertTrue(True)

    def test_GET_getset(self):
        self.assertEqual(self.req.GET, {})

        self.req.GET['key'] = 'value'
        self.assertEqual(self.req.GET['key'], 'value')

        self.req.GET = {'a': 'b'}
        self.assertEqual(self.req.GET, {'a': 'b'})

    def test_POST_getset(self):
        self.assertEqual(self.req.POST, {})

        self.req.POST['key'] = 'value'
        self.assertEqual(self.req.POST['key'], 'value')

        self.req.POST = {'a': 'b'}
        self.assertEqual(self.req.POST, {'a': 'b'})

    def test_headers_getset(self):
        self.assertEqual(self.req.headers, {})

        self.req.headers['key'] = 'value'
        self.assertEqual(self.req.headers['key'], 'value')

        self.req.headers = {'a': 'b'}
        self.assertEqual(self.req.headers, {'a': 'b'})

    def test_body_getset(self):
        self.req.body = 'body'
        self.assertEqual(self.req.body, 'body')

    def test_path_extract(self):
        self.req.url = '/?id=10&name=Rock'
        self.assertEqual(self.req.path, '/')

    def test_path_getset(self):
        self.req.path = '/about'
        self.assertEqual(self.req.path, '/about')


class TestRequestInit(unittest.TestCase):

    def setUp(self):
        self.req = guava.request.Request(url='/about',
                                         path='/about',
                                         host='www.google.com',
                                         method='POST',
                                         body='Hello World',
                                         headers={'UserAgent': 'xxx'},
                                         GET={'a': 10},
                                         POST={'a':20})

        self.req2 = guava.request.Request(url='/about',
                                         host='www.google.com',
                                         method='POST',
                                         body='Hello World',
                                         headers={'UserAgent': 'xxx'},
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

    def test_headers(self):
        self.assertEqual(self.req.headers, {'UserAgent': 'xxx'})

    def test_GET(self):
        self.assertEqual(self.req.GET, {'a': 10})

    def test_POST(self):
        self.assertEqual(self.req.POST, {'a': 20})


if __name__ == '__main__':
    unittest.main()
