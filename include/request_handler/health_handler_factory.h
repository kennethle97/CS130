
#ifndef HEALTH_HANDLER_FACTORY_H
#define HEALTH_HANDLER_FACTORY_H 

#include "request_handler/request_handler_health.h"
#include "config_parser.h"

class Health_Handler_Factory : public Request_Handler_Factory {
    public:
        Health_Handler_Factory(NginxConfig config);
        Request_Handler_Health* create(const std::string& location, const std::string& url);

    private:
        NginxConfig config;
        bool parse_config(NginxConfig config, std::string location);
};

#endif