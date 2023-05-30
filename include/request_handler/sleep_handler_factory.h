

#ifndef SLEEP_HANDLER_FACTORY_H
#define SLEEP_HANDLER_FACTORY_H 

#include "request_handler/request_handler_sleep.h"
#include "config_parser.h"

class Sleep_Handler_Factory : public Request_Handler_Factory {
    public:
        Sleep_Handler_Factory(NginxConfig config);
        Request_Handler_Sleep* create(const path_uri& location, const path_uri& url);

    private:
        NginxConfig config;
        bool parse_config(NginxConfig config, std::string location);
};

#endif