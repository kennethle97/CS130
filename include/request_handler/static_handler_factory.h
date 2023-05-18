

#ifndef STATIC_HANDLER_FACTORY_H
#define STATIC_HANDLER_FACTORY_H 

#include "request_handler/request_handler_static.h"
#include "config_parser.h"

class Static_Handler_Factory : public Request_Handler_Factory {
    public:
        Static_Handler_Factory(NginxConfig config);
        Request_Handler_Static* create(const path_uri& location, const path_uri& url);
        
    private:
        NginxConfig config;
        std::string parse_config(NginxConfig config, std::string location);
};

#endif