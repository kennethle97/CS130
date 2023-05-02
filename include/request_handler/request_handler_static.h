/* Define top-level class for request handlers to be able to add more in the future if necessary */

#ifndef REQUEST_HANDLER_STATIC_H
#define REQUEST_HANDLER_STATIC_H

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "../request_handler.h"

class Request_Handler_Static : public Request_Handler {
public:
    explicit Request_Handler_Static(const PathUri& root_, const PathUri& prefix_);
    void handle_request(const request& request, response* response) noexcept override;

private:
    PathUri root;    // Root path to serve files from
    PathUri prefix;  // Prefix in case no root
};

#endif
