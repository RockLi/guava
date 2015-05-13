#!/usr/bin/env python

import guava

class IndexController(guava.controller.Controller):

    def index(self):
        self.set_header('Custom-Header', 'Rock Lee')
        self.write('Hello World')
        import time
        time.sleep(10)

    def say_hello(self):
        self.write('My name is Rock Lee')
