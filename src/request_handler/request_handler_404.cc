
#include "../../include/request_handler/request_handler_404.h"
#include "logger.h"


void Request_Handler_404::handle_request(const request& http_request, reply* http_reply) noexcept {

    ServerLogger* server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("404 Request Handler: " + http_request.uri);
    *http_reply = reply::stock_reply(reply::not_found);
    return;
}

