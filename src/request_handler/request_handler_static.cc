
#include "../../include/request_handler/request_handler_static.h"
#include "../../include/http/mime_types.hpp"

Request_Handler_Static::Request_Handler_Static(const path_uri& root_, const path_uri& prefix_)
    : root(root_), prefix(prefix_) {}

void Request_Handler_Static::handle_request(const request &http_request, reply *http_reply) noexcept {
    
    
    // Create the full path of the file requested by combining the prefix and the request target
    std::string uri = http_request.uri;
    std::size_t prefix_pos = uri.find(prefix);
    if (prefix_pos != std::string::npos) {
        uri.replace(prefix_pos, prefix.length(), root);
    } else {
        *http_reply = reply::stock_reply(reply::not_found);
        return;
    }

    std::cout << "Static Request Handler Serving file: " << uri << std::endl;
    // Check if the requested file exists and is a regular file
    boost::filesystem::path path(uri);
    if (!boost::filesystem::exists(path)||!boost::filesystem::is_regular_file(uri)) {
       *http_reply = reply::stock_reply(reply::not_found);
        return;
    }

    // If the requested file exists, open it and read its contents
    boost::filesystem::ifstream file(path);
    std::stringstream file_stream;
    file_stream << file.rdbuf();
    std::string file_contents = file_stream.str();

    // Get the extension for the mimetype
    boost::filesystem::path boost_path(uri);
    std::string extension = boost_path.extension().string();

    http_reply->status = reply::ok;
    http_reply->headers.resize(2);
    http_reply->content = file_contents;
    http_reply->headers[0].name = "Content-Length";
    http_reply->headers[0].value = std::to_string(file_contents.length());
    http_reply->headers[1].name = "Content-Type";
    http_reply->headers[1].value = http::server::mime_types::extension_to_type(extension);
}
