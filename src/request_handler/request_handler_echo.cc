
#include "../../include/request_handler/request_handler_echo.h"



Request_Handler_Echo::Request_Handler_Echo(){}

void Request_Handler_Echo::handle_request(const request& request, response* response) {

    // Get the request message body and add it to the response message body
    response->body() = request.body();

    // Set the response status to OK (200)
    response->result(http::status::ok);

    // Set the response HTTP version to match the version of the incoming request
    response->version(request.version());

    // Set the content-type header of the response to "text/plain"
    response->set(http::field::content_type, "text/plain");

    // Set the content-length header of the response
    response->set(http::field::content_length, std::to_string(response->body().size()));
    
    // Set the response headers to indicate that the connection should be closed
    response->set(http::field::connection, "close");
}

