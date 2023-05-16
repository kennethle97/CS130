

#ifndef REQUEST_HANDLER_DISPATCH_H
#define REQUEST_HANDLER_DISPATCH_H 

#include <iostream>
#include <map>
#include <memory>
#include "request_handler.h"

typedef std::string path_uri;
typedef std::string path_handler_name;

#define ECHO_HANDLER "EchoHandler"
#define STATIC_HANDLER "StaticHandler"

class Request_Handler_Dispatcher {
public:
    Request_Handler_Dispatcher(const NginxConfig& config);
    std::shared_ptr<Request_Handler> get_request_handler(const request& http_request) const;

private:
    std::map<path_uri, std::shared_ptr<Request_Handler> > map_handlers;
    void parse_config_handlers(const NginxConfig& config);
};

#endif