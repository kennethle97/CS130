/* Define top-level class for request handlers to be able to add more in the future if necessary */

#ifndef REQUEST_HANDLER_STATIC_H
#define REQUEST_HANDLER_STATIC_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "../request_handler.h"
#include "../request_handler_dispatcher.h"

class Request_Handler_Static : public Request_Handler {
public:
    explicit Request_Handler_Static(const path_uri & root, const path_uri & location, const path_uri& url);
    void handle_request(const request& http_request, reply* http_reply) noexcept override;
    path_uri get_prefix() const {return location;}
    path_uri get_root() const {return root;}

private:
    std::string get_file(boost::filesystem::path path);
    path_uri root;    // Root path to serve files from
    path_uri location;  // Location to match with root directory
    path_uri url;   // Full path from http request
};

#endif
