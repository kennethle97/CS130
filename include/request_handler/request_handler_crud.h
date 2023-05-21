/*Define top level class for request_handlers to be able to add more in the future if necessary*/

#ifndef REQUEST_HANDLER_CRUD_H
#define REQUEST_HANDLER_CRUD_H
#include <iostream>
#include "../request_handler.h"
#include "../request_handler_dispatcher.h"

class Request_Handler_Crud: public Request_Handler {
    public:
        Request_Handler_Crud(const path_uri& data_path_, const path_uri& location_, const path_uri& url_);
        void handle_request(const request &http_request, reply *http_reply)noexcept override;
        
        path_uri get_data_path() const {return data_path_;}
    private:
        void create(const request &http_request, reply *http_reply);
        void read(const request &http_request, reply *http_reply);
        void update(const request &http_request, reply *http_reply);
        void del(const request &http_request, reply *http_reply);

        path_uri data_path_;    // Root path to serve files from
        path_uri location_;  // Location to match with root directory
        path_uri url_;   // Full path from http request
};



#endif