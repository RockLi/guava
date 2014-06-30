import cherrypy


class HelloWorld(object):
    def index(self):
        return "Hello World!"
    index.exposed = True


cherrypy.config.update({'server.socket_port': 8000})
cherrypy.config.update({'log.screen': False})
cherrypy.quickstart(HelloWorld())
