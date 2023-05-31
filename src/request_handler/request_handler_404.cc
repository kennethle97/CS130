
#include "../../include/request_handler/request_handler_404.h"
#include "logger.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

Request_Handler_404::Request_Handler_404(const path_uri& prefix_) 
    : prefix(prefix_) {}

void Request_Handler_404::handle_request(const request& http_request, reply* http_reply) noexcept {

    ServerLogger* server_logger = ServerLogger::get_server_logger();
    std::string uri = {http_request.target().begin(), http_request.target().end()};
    server_logger->log_debug("404 Request Handler: " + uri);
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
    server_logger->log_info("[HandlerMetrics] 404_handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
    return;
}

