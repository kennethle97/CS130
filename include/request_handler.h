/*Define top level class for request_handlers to be able to add more in the future if necessary*/

#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <iostream>
#include <boost/beast/http.hpp>
#include "./config_parser.h"

namespace http = boost::beast::http;

class Request_Handler {
    public:
        virtual void handle_request(const http::request<http::string_body>&request_ ,http::response<http::string_body> *response_) const = 0;

    protected:

};



#endif