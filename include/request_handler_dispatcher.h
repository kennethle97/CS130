

#ifndef REQUEST_HANDLER_DISPATCH_H
#define REQUEST_HANDLER_DISPATCH_H 

#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include "request_handler.h"
#include "request_handler/request_handler_factory.h"


typedef std::string path_handler_name;

#define ECHO_HANDLER "EchoHandler"
#define STATIC_HANDLER "StaticHandler"
#define CRUD_HANDLER "CRUDHandler"
#define HEALTH_HANDLER "HealthHandler"
#define SLEEP_HANDLER "SleepHandler"
#define MEME_HANDLER "MemeHandler"

class Request_Handler_Dispatcher {
public:
    Request_Handler_Dispatcher(const NginxConfig& config);
    std::shared_ptr<Request_Handler_Factory> get_request_handler_factory(const request& http_request) const;
    std::string match(const request &http_request) const;

private:
    std::map<path_uri, std::shared_ptr<Request_Handler_Factory> > map_handlers;
    void parse_config_handlers(const NginxConfig& config);
};

#endif