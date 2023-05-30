/* Define top-level class for request handlers to be able to add more in the future if necessary */

#ifndef REQUEST_HANDLER_Health_H
#define REQUEST_HANDLER_Health_H


#include <iostream>
#include "../request_handler.h"
#include "../request_handler_dispatcher.h"

class Request_Handler_Health : public Request_Handler {
public:
    Request_Handler_Health(const path_uri& prefix_);
    void handle_request(const request& http_request, reply* http_reply) noexcept override;

private:
    path_uri prefix;
};

#endif
