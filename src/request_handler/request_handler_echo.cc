
#include "../../include/request_handler/request_handler_echo.h"



void Request_Handler_Echo::handle_request(const request& http_request, reply* http_reply)noexcept {
    std::ostringstream request_body;
    request_body << http_request.method << " " << http_request.uri << " ";
    request_body << "HTTP/" << http_request.http_version_major << "." << http_request.http_version_minor << "\r\n";

    for (const auto& header : http_request.headers) {
        request_body << header.name << ": " << header.value << "\r\n";
    }

    request_body << "\r\n";
    std::string request_body_str = request_body.str();
    std::string request_body_length = std::to_string(request_body_str.length());

    http_reply->status = reply::ok;
    http_reply->headers.resize(2);
    http_reply->content = request_body.str();
    http_reply->headers[0].name = "Content-Length";
    http_reply->headers[0].value = request_body_length;
    http_reply->headers[1].name = "Content-Type";
    http_reply->headers[1].value = "text_plain";
}

