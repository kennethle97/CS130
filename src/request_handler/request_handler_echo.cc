
#include "../../include/request_handler/request_handler_echo.h"
#include "logger.h"

Request_Handler_Echo::Request_Handler_Echo(const path_uri& prefix_) 
    : prefix(prefix_) {}

void Request_Handler_Echo::handle_request(const request& http_request, reply* http_reply)noexcept {

    ServerLogger* server_logger = ServerLogger::get_server_logger();
    server_logger->log_trace("Echo Request Handler: " + http_request.uri);

    std::string request_body_str = get_request_body(http_request);
    std::string request_body_length = std::to_string(request_body_str.length());
    
    /*Set http_reply member values with the body of the request and set the reply to ok*/

    http_reply->status = reply::ok;
    http_reply->headers.resize(2);
    http_reply->content = request_body_str;
    http_reply->headers[0].name = "Content-Length";
    http_reply->headers[0].value = request_body_length;
    http_reply->headers[1].name = "Content-Type";
    http_reply->headers[1].value = "text/plain";
}

/*Takes an http_request object and converts contents into a response string*/
std::string Request_Handler_Echo::get_request_body(const request &http_request){
    std::ostringstream request_body;
    request_body << http_request.method << " " << http_request.uri << " ";
    request_body << "HTTP/" << http_request.http_version_major << "." << http_request.http_version_minor << "\r\n";

    for (const auto& header : http_request.headers) {
        request_body << header.name << ": " << header.value << "\r\n";
    }
    
    request_body << "\r\n";
    std::string request_body_str = request_body.str();
    return request_body_str;
}
