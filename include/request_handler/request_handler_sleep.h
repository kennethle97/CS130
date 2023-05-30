/*Define top level class for request_handlers to be able to add more in the future if necessary*/

#ifndef REQUEST_HANDLER_SLEEP_H
#define REQUEST_HANDLER_SLEEP_H
#include <iostream>
#include "../request_handler.h"
#include "../request_handler_dispatcher.h"

class Request_Handler_Sleep: public Request_Handler {
    public:
        Request_Handler_Sleep(const path_uri &prefix);
        void handle_request(const request &http_request, reply *http_reply)noexcept override;
        path_uri get_prefix() const {return prefix;}
    private:
        std::string get_request_body(const request &http_request);
        path_uri prefix;  // Prefix to match with root directory
};



#endif