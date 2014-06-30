import guava

class IndexController(guava.controller.Controller):

    def index(self):
        self.write("Hello World!")
