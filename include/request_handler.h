/*Define top level class for request_handlers to be able to add more in the future if necessary*/

#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <iostream>
#include <boost/beast/http.hpp>
#include "./config_parser.h"
#include "http/request.hpp"
#include "http/reply.hpp"
#include "http/request_parser.hpp"


using http::server::request;
using http::server::request_parser;
using http::server::reply;

class Request_Handler {
    public:
        virtual void handle_request(const request & http_request,reply * http_reply) = 0;

    protected:

};



#endif