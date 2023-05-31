
#include "../../include/request_handler/request_handler_health.h"
#include "logger.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

Request_Handler_Health::Request_Handler_Health(const path_uri& prefix_) 
    : prefix(prefix_) {}

void Request_Handler_Health::handle_request(const request& http_request, reply* http_reply) noexcept {

    ServerLogger* server_logger = ServerLogger::get_server_logger();
    std::string uri = {http_request.target().begin(), http_request.target().end()};
    server_logger->log_debug("Health Request Handler: " + uri);

    //*******stock reply************
    http_reply->result(boost::beast::http::status::ok);
    http_reply->body() = "OK";
    http_reply->content_length(http_reply->body().size());
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    //*******stock reply************
    server_logger->log_info("static file found");
    server_logger->log_info("[HandlerMetrics] Health_handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
    return;
}

