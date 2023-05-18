#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "../../include/request_handler/request_handler_echo.h"
#include "logger.h"

Request_Handler_Echo::Request_Handler_Echo(const path_uri& prefix_) 
    : prefix(prefix_) {}

void Request_Handler_Echo::handle_request(const request& http_request, reply* http_reply)noexcept {

    ServerLogger* server_logger = ServerLogger::get_server_logger();

    std::string uri = {http_request.target().begin(),http_request.target().end()};
    server_logger->log_trace("Echo Request Handler: " +  uri);

    /*Set http_reply member values with the body of the request and set the reply to ok*/
    http_reply->result(boost::beast::http::status::ok);
    http_reply->body()=boost::lexical_cast<std::string>(http_request.base());
    http_reply->set(boost::beast::http::field::content_length, std::to_string(http_reply->body().size()));
    http_reply->set(boost::beast::http::field::content_type, "text/plain");

    

}
