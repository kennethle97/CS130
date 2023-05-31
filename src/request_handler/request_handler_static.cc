
#include "../../include/request_handler/request_handler_static.h"
#include "../../include/http/mime_types.hpp"
#include "logger.h"

Request_Handler_Static::Request_Handler_Static(const path_uri& root_, const path_uri& location_, const path_uri& url_)
    : root(root_), location(location_), url(url_) {}

void Request_Handler_Static::handle_request(const request &http_request, reply *http_reply) noexcept {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    
    // Create the full path of the file requested by combining the location and the request target
    std::string uri = this->url;

    // remove any trailing backslashes from uri
    while (uri.back() == '/') {
        uri.pop_back();
    }

    std::size_t location_pos = uri.find(this->location);
    if (location_pos != std::string::npos) {
        uri.replace(location_pos, this->location.length(), root);
    } else {
        //*******stock reply************
        http_reply->result(boost::beast::http::status::not_found);
            const char not_found[] =
                        "<html>"
                        "<head><title>Not Found</title></head>"
                        "<body><h1>404 Not Found</h1></body>"
                        "</html>\n";
        http_reply->body() = not_found;
        http_reply->content_length(http_reply->body().size());
        http_reply->set(boost::beast::http::field::content_type, "text/html");
        //*******stock reply************
        server_logger->log_trace("-- static file location not found");
        server_logger->log_info("[HandlerMetrics] Static_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
        return;
    }
    // uri = "../../public/" + uri;
    server_logger->log_trace("Static Request Handler Serving file: " + uri);

    // Check if the requested file exists and is a regular file
    boost::filesystem::path path(uri);
    
    if (!boost::filesystem::exists(path)||!boost::filesystem::is_regular_file(uri)) {
        //*******stock reply************
        http_reply->result(boost::beast::http::status::not_found);
            const char not_found[] =
                        "<html>"
                        "<head><title>Not Found</title></head>"
                        "<body><h1>404 Not Found</h1></body>"
                        "</html>\n";
        http_reply->body() = not_found;
        http_reply->content_length(http_reply->body().size());
        http_reply->set(boost::beast::http::field::content_type, "text/html");
        //*******stock reply************
        server_logger->log_trace("-- static file not found");
        server_logger->log_info("[HandlerMetrics] Static_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
        return;
    }

    // If the requested file exists, open it and read its contents
    std::string file_contents = get_file(path);
    std::string file_size = std::to_string(file_contents.length()); 

    // Get the extension for the mimetype
    std::string extension = path.extension().string();
    http_reply->result(boost::beast::http::status::ok);
    http_reply->body()=file_contents;
    http_reply->set(boost::beast::http::field::content_length, file_size);
    http_reply->set(boost::beast::http::field::content_type, http::server::mime_types::extension_to_type(extension));
    server_logger->log_trace("-- static file found");
    server_logger->log_info("[HandlerMetrics] Static_handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));

}

/*get_file gets the file given the path of the file taking in a boost::filesystem::path path variable*/
std::string Request_Handler_Static::get_file(boost::filesystem::path path){
    boost::filesystem::ifstream file(path);
    std::stringstream file_stream;
    file_stream << file.rdbuf();
    std::string file_contents = file_stream.str();
    return file_contents;

}
