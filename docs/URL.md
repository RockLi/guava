# guava.url

## Methods

1. parse_url(url)

## Examples

```
u = guava.url.parse_url('http://google.com?name=rock#id1')
u.schema = 'http'
u.username = None
u.password = None
u.host = 'google.com'
u.path = '/'
u.query = {'name': 'rock'}
u.fragment = 'id1'

```


# guava.url.URL

## Properties

1. scheme: string, defaults to 'http'
3. username: string, defaults to None
4. password: string, defaults to None
5. host: string, defaults to 'localhost'
6. port: int, defaults to 80
7. path: string, defaults to '/'
8. queries: dict, defaults to None
9. fragment: string, defaults to None

## Methods

1. set_query(key, value)
2. __str__
3. decoded(): string

## Examples

1.

```
url = guava.url.URL(schema='http', username=None, password=None,
                    host='localhost', port=3000, path='/',
                    queries={
                        'name': 'rock'
                    }, fragment='id1')
```
