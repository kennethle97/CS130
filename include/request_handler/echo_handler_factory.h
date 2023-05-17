

#ifndef ECHO_HANDLER_FACTORY_H
#define ECHO_HANDLER_FACTORY_H 

#include "request_handler/request_handler_echo.h"

class Echo_Handler_Factory : public Request_Handler_Factory {
    public:
        Echo_Handler_Factory(NginxConfig config);
        Request_Handler_Echo* create(const path_uri& location, const path_uri& url);

    private:
        NginxConfig config;
};

#endif