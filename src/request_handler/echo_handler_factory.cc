#include "request_handler/request_handler_echo.h"
#include "request_handler/echo_handler_factory.h"

Echo_Handler_Factory::Echo_Handler_Factory(NginxConfig config) {
    this->config = config;
}

Request_Handler_Echo* Echo_Handler_Factory::create(const path_uri& location_, const path_uri& url_) {
    return new Request_Handler_Echo(location_);
}