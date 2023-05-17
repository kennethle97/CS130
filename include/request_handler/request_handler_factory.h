

#ifndef REQUEST_HANDLER_FACTORY_H
#define REQUEST_HANDLER_FACTORY_H 

#include "request_handler.h"

typedef std::string path_uri;

class Request_Handler_Factory {
    public:

        virtual Request_Handler* create(const path_uri& location, const path_uri& url) = 0;

};

#endif