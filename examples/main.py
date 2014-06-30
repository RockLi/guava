#!/usr/bin/env python

import guava

class MyRouter(guava.router.CustomRouter):

    def __init__(self):
        super(MyRouter, self).__init__()
        self.register_handler(path='/hello', method='GET', package='.', controller='IndexController', action='hello_msg')

    def route(self, req):
        if req.path == '/me' and req.GET['name'] == 'rock':
            return guava.handler.Handler(package='.',
                                         module='me',
                                         cls='MeController',
                                         action='show')

        return None


if __name__ == "__main__":
	session_store = guava.session.MemStore()

    server = guava.server.Server(ip="0.0.0.0",
                                 port=8000,
                                 backlog=128,
                                 auto_reload=True,
								 session_store=session_store)

    mvc_router = guava.router.MVCRouter(path="/", enable_session=True)

    static_router = guava.router.StaticRouter(path="/static", directory="my_static_dir", allow_index=True)

    api_router = guava.router.RESTRouter(path="/api", enable_session=False)

    #misc_router = guava.router.CustomRouter({
    #    "/about": guava.handler.Handler(package=".", module='me', cls="MiscController", action= "about")
    #})

    #my_router = MyRouter()

    #server.add_router(mvc_router, static_router, api_router, misc_router, my_router)
    server.add_router(static_router)

    server.serve()
