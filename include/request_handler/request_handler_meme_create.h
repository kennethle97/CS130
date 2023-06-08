/* Define top-level class for request handlers to be able to add more in the future if necessary */

#ifndef REQUEST_HANDLER_MEME_CREATE_H
#define REQUEST_HANDLER_MEME_CREATE_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "../request_handler.h"
#include "../request_handler_dispatcher.h"
#include "../nlohmann/json.hpp"

class Request_Handler_Meme_Create : public Request_Handler {
public:
    explicit Request_Handler_Meme_Create(const path_uri & root, const path_uri & location, const path_uri& url);
    void handle_request(const request& http_request, reply* http_reply) noexcept override;

private:
    path_uri root;    // Root path to serve files from
    path_uri location;  // Location to match with root directory
    path_uri url;   // Full path from http request

    std::string use_configured_root(reply *http_reply);
    std::string update_field_id(std::string rootpath);
    bool isRequestValidJson(std::string json_data);
    void create_meme(std::string path, std::string json_data);
    void write_invalid_json_response(reply *http_reply);

};

#endif