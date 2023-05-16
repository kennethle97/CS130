
#include "../../include/request_handler/request_handler_static.h"
#include "../../include/http/mime_types.hpp"
#include "logger.h"

Request_Handler_Static::Request_Handler_Static(const path_uri& root_, const path_uri& prefix_)
    : root(root_), prefix(prefix_) {}

void Request_Handler_Static::handle_request(const request &http_request, reply *http_reply) noexcept {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    
    // Create the full path of the file requested by combining the prefix and the request target
    std::string uri = http_request.uri;

    // remove any trailing backslashes from uri
    while (uri.back() == '/') {
        uri.pop_back();
    }

    std::size_t prefix_pos = uri.find(prefix);
    if (prefix_pos != std::string::npos) {
        uri.replace(prefix_pos, prefix.length(), root);
    } else {
        *http_reply = reply::stock_reply(reply::not_found);
        server_logger->log_info("Bad request -- prefix not found");
        return;
    }
    // uri = "../../public/" + uri;
    server_logger->log_trace("Static Request Handler Serving file: " + uri);

    // Check if the requested file exists and is a regular file
    boost::filesystem::path path(uri);
    
    if (!boost::filesystem::exists(path)||!boost::filesystem::is_regular_file(uri)) {
        *http_reply = reply::stock_reply(reply::not_found);
        server_logger->log_info("Bad request -- file not found");
        return;
    }

    // If the requested file exists, open it and read its contents
    std::string file_contents = get_file(path);
    std::string file_size = std::to_string(file_contents.length()); 

    // Get the extension for the mimetype
    std::string extension = path.extension().string();

    http_reply->status = reply::ok;
    http_reply->headers.resize(2);
    http_reply->content = file_contents;
    http_reply->headers[0].name = "Content-Length";
    http_reply->headers[0].value = file_size;
    http_reply->headers[1].name = "Content-Type";
    http_reply->headers[1].value = http::server::mime_types::extension_to_type(extension);
}

/*get_file gets the file given the path of the file taking in a boost::filesystem::path path variable*/
std::string Request_Handler_Static::get_file(boost::filesystem::path path){
    boost::filesystem::ifstream file(path);
    std::stringstream file_stream;
    file_stream << file.rdbuf();
    std::string file_contents = file_stream.str();
    return file_contents;

}
