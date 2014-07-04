#!/usr/bin/env python

import guava

class MyRouter(guava.router.Router):

    def __init__(self, *args, **kwargs):
        super(MyRouter, self).__init__(*args, **kwargs)

    def route(self, req):
        if req.path == '/me' and req.GET['name'] == 'rock':
            return guava.handler.Handler(package='.',
                                         module='me',
                                         cls='MeController',
                                         action='show')

        return None


if __name__ == "__main__":
    session_store = guava.session.SessionStore(type=guava.session.Mem)

    server = guava.server.Server(ip="0.0.0.0",
                                 port=8000,
                                 backlog=128,
                                 auto_reload=True)

    mvc_router = guava.router.MVCRouter(mount_point="/",
                                        package='controllers',
                                        session_store=session_store)

    static_router = guava.router.StaticRouter(mount_point="/static", directory="my_static_dir", allow_index=True)

    api_router = guava.router.RESTRouter(mount_point="/api", session_store=None)

    misc_router = guava.router.Router({
        "/about": guava.handler.Handler(package=".", module='me', cls="MiscController", action= "about")
    })

    my_router = MyRouter()

    server.add_router(mvc_router, static_router, api_router, misc_router, my_router)

    server.serve()
