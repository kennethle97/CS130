
#include "../../include/request_handler/request_handler_static.h"

Request_Handler_Static::Request_Handler_Static(const PathUri& root_, const PathUri& prefix_)
    : root(root_), prefix(prefix_) {}

void Request_Handler_Static::handleRequest(const request &request_, response *response) noexcept {
    // Create the full path of the file requested by combining the prefix and the request target
    std::string file_path = root + request_.target().to_string().substr(prefix.length());

    // Check if the requested file exists
    boost::filesystem::path path(file_path);
    if (!boost::filesystem::exists(path)) {
        response_->result(http::status::not_found);
        rresponse_->set(http::field::content_type, "text/plain");
        response_->body() = "File not found.";
        return;
    }

    // If the requested file exists, open it and read its contents
    boost::filesystem::ifstream file(path);
    std::stringstream file_contents;
    file_contents << file.rdbuf();

    // Set the response status to OK (200) and set the body of the response to the contents of the requested file
    response_->result(http::status::ok);
    response_->set(http::field::content_type, "text/plain");
    response_->body() = file_contents.str();
    response_->set(http::field::content_length, std::to_string(reply_->body().size()));
}
