# guava.request.Request



## Properties

1. method: string, defaults to "GET"
2. url: guava.url.URL
3. proto: double, defaults to 1.0
4. proto_major: int, defaults to 1
5. proto_minor: int, defaults to 0
6. body: string, defaults to ''
7. HEADERS: dict, defaults to None
8. GET: dict, defaults to None
9. POST: dict, defaults to None
10. COOKIES: dict, defaults to None
11. SESSION: dict, defaults to None

## Methods

1. set_header(name, value)
2. set_cookie(name, value, expire=0, path='/', domain=None, secure=False, httponly=False)
3. set_session(name, value)

## Examples

1.

```
req = guava.request.Request(method='GET', proto='HTTP/1.0',
                            url='')

```
