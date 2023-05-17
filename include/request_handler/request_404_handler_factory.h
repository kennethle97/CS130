

#ifndef REQUEST_404_HANDLER_FACTORY_H
#define REQUEST_404_HANDLER_FACTORY_H 

#include "request_handler/request_handler_404.h"

class Request_404_Handler_Factory : public Request_Handler_Factory {
    public:
        Request_404_Handler_Factory() {};
        Request_Handler_404* create(const std::string& location, const std::string& url);
};

#endif