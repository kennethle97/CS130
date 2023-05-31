#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "../../include/request_handler/request_handler_sleep.h"
#include "logger.h"

Request_Handler_Sleep::Request_Handler_Sleep(const path_uri& prefix_) 
    : prefix(prefix_) {}

void Request_Handler_Sleep::handle_request(const request& http_request, reply* http_reply)noexcept {
    sleep(7); // sleeping for 4 seconds
    ServerLogger* server_logger = ServerLogger::get_server_logger();

    std::string uri = {http_request.target().begin(),http_request.target().end()};
    server_logger->log_trace("Sleep Request Handler: " +  uri);

    /*Set http_reply member values with the body of the request and set the reply to ok*/
    http_reply->result(boost::beast::http::status::ok);
    const char sleeping[] =
                        "<html>"
                        "<head><title>Sleeping</title></head>"
                        "<body><h1>I had a good sleep for 10 seconds!</h1></body>"
                        "</html>\n";
    http_reply->body()=sleeping;
    http_reply->set(boost::beast::http::field::content_length, std::to_string(http_reply->body().size()));
    http_reply->set(boost::beast::http::field::content_type, "text/plain");

    

}
