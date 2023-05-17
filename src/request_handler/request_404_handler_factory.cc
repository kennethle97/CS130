#include "request_handler/request_handler_404.h"
#include "request_handler/request_404_handler_factory.h"

Request_404_Handler_Factory::Request_404_Handler_Factory(NginxConfig config) {
    this->config = config;
}

Request_Handler_404* Request_404_Handler_Factory::create(const std::string& location_, const std::string& url) {
    return new Request_Handler_404();
}