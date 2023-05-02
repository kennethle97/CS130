

#ifndef REQUEST_HANDLER_DISPATCH_H
#define REQUEST_HANDLER_DISPATCH_H 

#include <iostream>
#include <map>
#include <memory>
#include "request_handler.h"

class Request_Handler_Dispatcher {
public:
    Request_Handler_Dispatcher(const NginxConfig& config);
    std::shared_ptr<const Request_Handler> get_request_handler(const http::request<http::string_body>& request) const;

private:
    std::map<std::string, std::shared_ptr<Request_Handler> > map_handlers;
    void parse_config_handlers(const NginxConfig& config);
};

#endif