
#include "../../include/request_handler/request_handler_crud.h"
#include "../../include/http/mime_types.hpp"
#include "logger.h"

Request_Handler_Crud::Request_Handler_Crud(const path_uri& data_path_, const path_uri& location_, const path_uri& url_) {
    this->data_path_ = data_path_;
    this->location_ = location_;
    this->url_ = url_;
}

void Request_Handler_Crud::handle_request(const request &http_request, reply *http_reply) noexcept {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    boost::beast::string_view method = http_request.method_string();
    if (method == "POST") {
        create(http_request, http_reply);
    } else if (method == "GET") {
        read(http_request, http_reply);
    } else if (method == "PUT") {
        update(http_request, http_reply);
    } else if (method == "DELETE") {
        del(http_request, http_reply);
    } else {
        // TODO: Handle invalid request method
        server_logger->log_error("Invalid request method");
        return;
    }
    return;
}

void Request_Handler_Crud::create(const request &http_request, reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("Request_Handler_Crud::create()");

}

void Request_Handler_Crud::read(const request &http_request, reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("Request_Handler_Crud::read()");
}

void Request_Handler_Crud::update(const request &http_request, reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("Request_Handler_Crud::update()");
}

void Request_Handler_Crud::del(const request &http_request, reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("Request_Handler_Crud::del()");
}