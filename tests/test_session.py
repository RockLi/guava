# Copyright 2014 The guava Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

import unittest
import guava


class TestSessionMem(unittest.TestCase):

    def setUp(self):
        self.session_store = guava.session.SessionStore(type=guava.session.Mem)

    def test_mem_store(self):
        session_id = '123456789'
		self.assertEqual(self.session_store.get(session_id), None)

		self.session_store.set(session_id, {'name': 'Rock', 'age': 26})
		self.assertEqual(self.session_store.get(session_id), {
			'name': 'Rock',
			'age': 26
		})

		self.session_store.delete(session_id)
		self.assertEqual(self.session_store.get(session_id), None)

		self.session_store.set(session_id, {'name': 'Rock'})
		self.session_store.clear(session_id)
		self.assertEqual(self.session_store.get(session_id), {})


class MySessionStore(guava.session.SessionStore):

	def __init__(self, *args, **kwargs):
		super(MySessionStore, self).__init__(*args, **kwargs)
		self.__items = {}

	def set(self, sid, value):
		self.__items[sid] = value

	def get(self, sid):
		return self.__items.get(sid)

	def delete(self, sid):
		del self.__items[sid]

	def clear(self, sid):
		self.__items[sid] = {}

class TestSessionCustom(unittest.TestCase):

	def setUp(self):
		self.session_store = MySessionStore()

	def test_basic(self):
		session_id = '123456789'
		self.assertEqual(self.session_store.get(session_id), None)

		self.session_store.set(session_id, {'name': 'Rock', 'age': 26})
		self.assertEqual(self.session_store.get(session_id), {
			'name': 'Rock',
			'age': 26
		})

		self.session_store.delete(session_id)
		self.assertEqual(self.session_store.get(session_id), None)

		self.session_store.set(session_id, {'name': 'Rock'})
		self.session_store.clear(session_id)
		self.assertEqual(self.session_store.get(session_id), {})


class TestSessionFile(unittest.TestCase):

	def setUp(self):
		self.session_store = guava.session.SessionStore(type=guava.session.File)

	def tearDown(self):
		self.session_store.delete('123456789')

	def test_basic(self):
		session_id = '123456789'
		self.assertEqual(self.session_store.get(session_id), None)

		self.session_store.set(session_id, {'name': 'Rock', 'age': 26})
		self.assertEqual(self.session_store.get(session_id), {
			'name': 'Rock',
			'age': 26
		})

		self.session_store.delete(session_id)
		self.assertEqual(self.session_store.get(session_id), None)

		self.session_store.set(session_id, {'name': 'Rock'})
		self.session_store.clear(session_id)
		self.assertEqual(self.session_store.get(session_id), {})


class TestSession(unittest.TestCase):

	def setUp(self):
		pass

	def test_sid(self):
		sid1 = guava.session.generate_id()
		sid2 = guava.session.generate_id()

		self.assertNotEqual(sid1, sid2)


if __name__ == '__main__':
	unittest.main()
