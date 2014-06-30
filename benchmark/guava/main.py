#!/usr/bin/env python

import guava

server = guava.server.Server(ip="0.0.0.0")

router = guava.router.MVCRouter('/')

server.add_router(router)

server.serve()

