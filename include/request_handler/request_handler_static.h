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
    explicit Request_Handler_Static(const path_uri & root, const path_uri & prefix);
    void handle_request(const request& http_request, reply* http_reply) noexcept override;

private:
    std::string get_file(boost::filesystem::path path);
    path_uri root;    // Root path to serve files from
    path_uri prefix;  // Prefix to match with root directory
};

#endif
