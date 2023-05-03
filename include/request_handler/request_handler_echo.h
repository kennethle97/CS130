/*Define top level class for request_handlers to be able to add more in the future if necessary*/

#ifndef REQUEST_HANDLER_ECHO_H
#define REQUEST_HANDLER_ECHO_H
#include <iostream>
#include "../request_handler.h"

class Request_Handler_Echo: public Request_Handler {
    public:
        Request_Handler_Echo() {};
        void handle_request(const request &http_request, reply *http_reply)noexcept override;
};



#endif