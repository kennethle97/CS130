# CS130 Web Server

## 1. Source Code Layout

The codebase is used to create a web server, implmemented in C++ with the help of the Boost library, that can echo HTTP requests and serve static files. The codebase is organized as follows:

### Server Initialization
The main file **server_main.cc** parses the config file **server.conf** via **config_parser.h** and creates the server object via **server.h**. Upon receiving a request, the server object creates a session object via **session.h**.

### Request Handling
The request handler dispatcher is created by **request_handler_dispatcher.h**, and is initialized in **server.h**. The dispatcher is responsible for dispatching incoming requests by matching the request url against a map of ```{paths: requestHandlerFactorys}``` as per the given [specifications](https://www.cs130.org/assignments/6/#dispatching).

Each request handler is produced by request handler factories. For echo, static, and 404 handlers, the factories are **echo_handler_factory.h**, **static_handler_factory.h**, and **request_404_handler_factory.h** respectively. The factories are initialized in **request_handler_dispatcher.h**, and their corresponding handlers are initialized in **request_handler_echo.h**, **request_handler_static.h**, and **request_handler_404.h** respectively.

- **request_handler_dispatcher.h**: dispatches incoming requests to a map of ```{paths: requestHandlerFactorys}```, and the resulting factories are then used to create request handlers, each with a short lifetime and matched to an incoming request
- **echo_handler_factory.h**: creates echo request handlers that echo the request back to the client in the response
- **static_handler_factory.h**: creates static request handlers that return the contents of a file specified in the request
- **request_404_handler_factory.h**: creates 404 request handlers
- **request_handler_echo.h**: handles echo requests
- **request_handler_static.h**: handles static requests
- **request_handler_404.h**: handles requests that do not match paths for echo or static requests
- **request_handler.h**: defines the interface for request handlers with a ```handle_request``` method that takes in a request and a response pointer, and creates a response based on the request

### HTTP
- **header.hpp**: defines the name : value pairs of HTTP headers
- **mime_types.hpp**: defines the mime types of HTTP headers, and converts file extensions to mime types
- **reply.hpp**: defines the HTTP reply object with status code, headers, and content
- **request_parser.hpp**: parses the HTTP request
- **request.hpp**: defines the HTTP request object with method, uri, http version, and headers

### Logging
- **logger.h**: defines the logger object that logs to console and file--used to log events at different severities (trace, debug, info, warning, error, fatal)

### Tests
Tests are located in the **/tests** directory. The current build has unit tests and an integration test. The unit tests are located in **/tests** and are written in C++ and Google Test (**gtest**). The integration tests are located in **/tests/integration_tests** and are written in shell script.

## 2. Build, Test, Run Instructions
### Build
To build the server binary and tests, run the following commands from the root directory of the project:
```
mkdir build
cd build
cmake ..
make
```

To build the docker image, run the following commands from the root directory of the project:
```
docker build -f docker/base.Dockerfile -t projects:base .
docker build -f docker/Dockerfile -t projects:latest .
```

Note that the base image, if unaltered, only needs to be built once.

To build the coverage docker image, run the following commands from the root directory of the project:
```
docker build -f docker/coverage.Dockerfile -t projects:coverage .
```

### Test
Tests can be run from the **/build** directory by running the following command after building:
```
make test
```
This will also run the integration tests.

To run a specific unit test, run the following command from the **/test** directory:
```
../build/bin/{unit_test_source_name}
```

For instance, running the following command will run the unit test for the static request handler:
```
../build/bin/request_handler_static_test
```

To run the integration tests, run the following commands from the root directory of the project:
```
cd tests/integration_tests
./integration_test.sh
```

### Run
To run the server binary, run the following commands from the **/build** directory:
```
cd bin
./server ../../server.conf
```
where **server.conf** is the config file for the server defined like so:
```
port 80;

location /echo EchoHandler {

}

location /static1 StaticHandler {    
    root ../../public/folder1; # absolute folder path
}

location /static2 StaticHandler {
    root ../../public/folder2; # absolute folder path
}

location / ErrorHandler {

}
```
The config file can be modified to any format defined by the Common API: [https://www.cs130.org/assignments/6/#config-file-format](https://www.cs130.org/assignments/6/#config-file-format)

To run the docker image, run the following commands from the root directory of the project:
```
docker run --rm -p 127.0.0.1:80:80 --name my_run projects:latest
```

To kill the server binary or docker container, ```ctrl-c``` or ```docker stop my_run```.

## 3. How to add a Request Handler
Suppose we want to add a new request handler **FooHandler** that handles requests to ```/foo``` without filesystem paths.

1. **Add the handler declaration to the config file**
  ```
  location /foo FooHandler {

  }
  ```

2. **Define the request handler factory**

_**ADD HERE**_

3. **Define the request handler**

_**ADD HERE**_


4. **Add the handler factory to the dispatcher**

_**ADD HERE**_


5. **Add the new handler to add_library in CMakeLists.txt**
  ```
  add_library(request_handler_lib 
                src/request_handler_dispatcher.cc
                src/request_handler/request_404_handler_factory.cc
                src/request_handler/static_handler_factory.cc
                src/request_handler/echo_handler_factory.cc
                src/request_handler/request_handler_echo.cc
                src/request_handler/request_handler_static.cc
                src/request_handler/request_handler_404.cc

                src/request_handler/foo_handler_factory.cc
                src/request_handler/request_handler_foo.cc
                )
  ```
6. **Add unit tests for the new request handler in tests**

_**ADD HERE**_
