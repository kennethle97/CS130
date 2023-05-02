/*Define top level class for request_handlers to be able to add more in the future if necessary*/

#ifndef REQUEST_HANDLER_ECHO_H
#define REQUEST_HANDLER_ECHO_H

#include "../request_handler.h"


class Request_Handler_Echo: public Request_Handler {
    public:
        noexcept explicit Request_Handler_Echo() {};
        void override handle_request(const http::request &request, http::response *response);
    private:
}



#endif