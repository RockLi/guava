# Guava [![Build Status](https://travis-ci.org/flatpeach/guava.svg)](https://travis-ci.org/flatpeach/guava)

Guava is a super lightweight high performance web framework for Python written in C. It is totally different with other traditional Python web frameworks. keep in mind, this project is not just revent the wheel.

If you don't like the features Guava supplied, you can use the underlying structures like **router**, **session**, **request**, **response**, **builtin web server** to write your own web framework!


I wrote an article to explain the design of the guava framework.

[Link here](http://code-trick.com/guava-a-super-lightweight-high-performance-web-framework-for-python-written-in-c/), please try to read that article, because this README.md maybe a little bit outdated.


## Status of this Project

Still under highly active development. The git repo is treated as **Preview Version**. So donot use it for production now.

I will release the stable version ASAP.

Anyways, you can evaluate it, hack it in advance. If you want to contribute, please see the contribution section.


## Philosophy of Guava

For the detailed story and design of the Guava project, please go to [my blog](http://www.code-trick.com) (**Haven't finished yet**)

1. High performance
2. Convention over configuration
3. Lightweight, only do what one web framework should do
4. Super scalability


## Main Features

1. Asynchorous, build on top of libuv
2. Builtin HTTP webserver
3. Builtin routers: StaticRouter, MVCRouter, CustomRouter, RESTFulRouter
4. Session Management: InMemory, Local FileSystem, Remote(SSO)
5. Everything is configurable


## Deployment


### Deploy behind of Nginx/Apache


```
------------                                ----------------------
| WebServer | <<-- Reversed Proxy Rule -->> | Guava HTTP Server |
------------                                ----------------------
```

### No Nginx/Apache

The performance of the Guava builtin web server is good enough for serving as the standalone web server. But till now I haven't spend so much time on the security part, so maybe it's not the best time to choose this kind of deployment.


## Router


Guava has four builtin routers trying to simplify your life. For detailed documentation, please refer to the doc directory in this repo.

Each router has one mount point.

### StaticRouter

StaticRouter is dedicated for serving static resources, for example: css, javascript files or images.


```
static_router = guava.router.StaticRouter(mount_point="/static",
                                          directory="my_static_dir",
                                          allow_index=True)
```

### MVCRouter

This could be set as the default router, if your application is such a typical one.

```
mvc_router = guava.router.MVCRouter(mount_point="/")
```

For exmaple:

URL  | Class  | Action
---- | ------ | ------
/    | IndexController | index
/user | UserController | index
/user/message | UserController | message


### RESTRouter

This router is especially useful if you want to supply the RESTFul apis.

Method | URL | Class | Action
------ | --- | ----- | ------
GET    | /users | UsersController | GET
GET  | /users/10 | UsersController |GET_ONE
DELETE | /users/10 | UsersController | DELETE
POST | /users/ | UsersController | POST
PUT | /users/10 | UsersController |PUT


### Customerize or implement advanced router

If above routers can not match all of your requirements, you can use CustomRouter to build or overwrite complex routes

```
custom_router = guava.router.Router({
	"/about": guava.handler.Handler(package='.',
                                    module='misc',
                                    controller='MiscController',
                                    action='about')
})
```


```
class MySpecialRouter(guava.router.Router):

    def __init__(self):
		self.register('/hello',
		              guava.handler.Handler(package='.',
                                            module='misc',
                                            controller='MiscController',
                                            action='hello'))

	def route(self, req):
		if req.path == '/me' and req.GET['name'] == 'rock':
			return guava.handler.Handler(package='.',
                                         module='me',
                                         controller='MeController',
                                         action='show')

		return None

```


## Controller

All your controllers should inherit from ```guava.controller.Controller```.



## Session

Guava already builtin two kinds of Session store solutions, one is in memory store, the other is file based store. If you want to support SSO and try to store session in MC, Redis or databases, you need to create a new class inherited from the builtin session store.

### InMemory Store

This is specially useful for debugging purpose. Each time you restarted the guava web server, all data in this kind of session store will be dropped.

```
session_store = guava.session.SessionStore(type=guava.session.Mem)
```


### File Based Store


```
session_store = guava.session.SessionStore(type=guava.session.File)
```

### Custom the Session Store

If you want to implement the SSO, you need the central based session storage solution by using Redis, memcache, database or other brokers to store the session data.

```
class RedisSessionStore(guava.session.SessionStore):

    def __init__(self, *args, **kwargs)):
        super(MySessionStore, self).__init__(*args, **kwargs)

	def set(self, sid, value):
        pass

	def get(self, sid):
        pass

	def delete(self, sid):
        pass

	def clear(self, sid):
        pass

```

You need to implement the four placeholder functions to build your own session store solution.


## Dependencies

Project | Description
----- | --------------------------------------
libuv | Cross-platform asychronous I/O library
http-parser | http request/response parser for c



## Install

### Install from Git

```git submodule update --init```

```cd deps/libuv; ./build.sh; ./configure; make; sudo make install```

```cd deps/http-parser; make```

```sudo python setup.py install```


## Performance

I will give out the performance result in my blog, will update this document afterwards.


## Contributions

To me, all kinds of contributions are welcome.

1. Contribute to the core codes
2. Testcases
3. Examples
4. Documentation
5. Website or the Logo for the Guava project
6. Even request for new features!
