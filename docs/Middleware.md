# Middleware


## Process in guava

You should already know that all your controller should inherit from ```guava.controller.Controller```.

1. before_action
2. [your action here]
3. after_action

Above steps are normally for one controller.

Process for integrated with middlewares:

1. middleware1:process_request
2. middleware2:process_request
3. [controller:before_action]
4. [controller:action]
5. [controller:after_action]
6. middleware2:process_response
7. middleware1:process_response

prototype for process_request:

```process_request(self, request)```


prototype for process_response:

```process_response(self, request, response)```


If you want to access the SESSION object, you can access it from the request object or the response, both carried the identical SESSION object.


The concept here is a little bit similiar to Django, if you return the Response object in those middlewares in advance, the process will stop at that point and return the ```Response``` to the client.

Notice:

1. All middlewares only will be instantilized for once.

## Code Samples


## Future

I will add some builtin middlewares sooner or later. Hope this feature can help you guys.
