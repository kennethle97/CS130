

#ifndef STATIC_HANDLER_FACTORY_H
#define STATIC_HANDLER_FACTORY_H 

#include "request_handler/request_handler_static.h"
#include "config_parser.h"

class Static_Handler_Factory : public Request_Handler_Factory {
    public:
        Static_Handler_Factory(NginxConfig config);
        Request_Handler_Static* create(const path_uri& location, const path_uri& url);
    private:
        std::string parse_config_root(NginxConfig config, std::string location);
        path_uri prefix;  // Prefix to match with root directory
        NginxConfig config;
};

#endif