# Guava [![Build Status](https://travis-ci.org/flatpeach/guava.svg)](https://travis-ci.org/flatpeach/guava)

Guava is a super lightweight and high performance web framework for Python written in C. It is totally different with other traditional Python web frameworks. Keep in mind, this project is not to revent the wheel.

If you don't like the features Guava supplied, you can use the underlying structures like **router**, **session**, **request**, **response**, **builtin web server** to construct your own web framework with the benifits of high performance which guava gives you.

I wrote an article to explain a little bit of the design of the guava framework, [Link](http://code-trick.com/guava-a-super-lightweight-high-performance-web-framework-for-python-written-in-c/).


## Status

1. Under highly active development.
2. Haven't release any stable versions, this git repo is the **Developer Preview Version**. Donot use it for production now.

I will release the stable version ASAP. I will also release the traditional web applications built on top of guava.

Anyways, you can evaluate it, hack it in advance. Don't forget to star it if you think guava can help you in the future. :)

If you want to contribute, please see the contribution section.

I will try my best to keep this README.md up to date, any time it's better if you can read the source codes.


## Philosophy of Guava

For the detailed story and design of the Guava project, please go to [My Blog](http://www.code-trick.com).

1. High performance
2. Prefer convention over configuration
3. Lightweight, only do what one web framework should do
4. Super scalability


## Main Features

1. Asynchorous, build on top of libuv
2. Builtin HTTP webserver
3. Builtin routers: Router, StaticRouter, MVCRouter, RESTFulRouter
4. Session Management: InMemory, Local FileSystem, Remote(SSO)
5. Everything is extensible


## Performance


I did a quick performance testing, all codes are stored in **benchmark** folder.

If anything is not correct, please kindly to correct me.


**Testing Environment**

EC2: t2.micro 1CPU 0.613GIB EBS

OS: Ubuntu14.04

Benchmark program: [wrk](https://github.com/wg/wrk)


1. Helloworld Performance

Command: ```wrk -t12 -c400 -d30s http://127.0.0.1:8000/```

This runs a benchmark for 30 seconds, using 12 threads, and keeping 400 HTTP connections open.

Already disabled al

| Framework |  Requests/s | Notes |
| --------- | ----------- | ------------ |
| Flask     |  595.73 | Actually failed to run the full testing |
| CherryPy  |  1627.68 |
| Tornado   |  3373.22 |
| NodeJS Raw |  4977.63 |
| Go Raw |  20230.32 |
| guava     |  18799.11 |  |


The reason why this time of testing guava didn't win Go is due to some known but unfixed bugs in guava, I will fix that soon and rerun the testing.

After I finished basic features, I will focus on the optimization part, continously to improve the performance. To be honest, there're lots of places in guava could be optimized.


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

Each router has one mount point. All routers will composite the tree like structures. The concept of mount point is for you easily group you sub applications.

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

|       URL      |     Package       | Class | Module | Action |  Args  |  GET  | POST |
| -------------- | ----------------- | ------ | ------ | ------ | ----- | ---- |
| /              | controllers       | IndexController | index  | index  | ()     | {}    | {}   |
| /post          | controllers       | IndexController | post   | index  | ()     | {}    | {}   |
| /post/new      | controllers       | IndexController | post   | new    | ()     | {}    | {}   |
| /post/view/10   | controllers       | IndexController | post   | view   | ('10',)  | {}  | {}  |
| /post/move/10/20 | controllers     | IndexController | post   | move   | ('10', '20',) | {} | {} |
| /post/edit/10?type=draft | controllers | IndexController | post | edit | ('10', ) | {'type': 'draft'} | {} |


### RESTRouter

This router is especially useful if you want to supply the RESTFul apis.

| Method | URL | Class | Action |
| ------ | --- | ----- | ------ |
| GET    | /users | UsersController | get_all |
| GET  | /users/10 | UsersController | get_one |
| DELETE | /users/10 | UsersController | delete_one |
| POST | /users/ | UsersController | create_one |
| PUT | /users/10 | UsersController | update_one |

**I havn't thought the best way to handler subresource like this kind of urls ```/users/10/friends/```, after I get a better idea, I will integrate with this feature soon.**

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

```sudo python setup.py install```

Run tests: ```python -m unittest discover```


### Install from github using pip

```sudo pip install -e git+https://github.com/flatpeach/guava.git#egg=guava```


### Install from PyPI

```sudo pip install guava```


## Quick Usage Tricks

1. Launch a web server at current directory

    ```python -c 'import guava; guava.server.start_static_server()'```

    It's the same as

    ```python -m SimpleHTTPServer```


## Performance

Please see my blog.


## Acknowledgements


1. [Jim Baker](https://github.com/jimbaker)

   Thanks for transfer the ownership of the name "guava" in PyPI.


## Contributions

To me, all kinds of contributions are welcome.

1. Contribute to the core codes
2. Testcases
3. Examples
4. Documentation
5. Website or the Logo for the Guava project
6. Even request for new features!
