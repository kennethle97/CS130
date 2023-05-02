
#include "../../include/request_handler/request_handler_static.h"

RequestHandlerStatic::RequestHandlerStatic(const PathUri& root_, const PathUri& prefix_)
    : root(root_), prefix(prefix_) {}

void RequestHandlerStatic::handleRequest(const request &request_, response *response) noexcept {
    // Create the full path of the file requested by combining the prefix and the request target
    std::string file_path = root + request_.target().to_string().substr(prefix.length());

    // Check if the requested file exists
    boost::filesystem::path path(file_path);
    if (!boost::filesystem::exists(path)) {
        reply_->result(http::status::not_found);
        reply_->set(http::field::content_type, "text/plain");
        reply_->body() = "File not found.";
        return;
    }

    // If the requested file exists, open it and read its contents
    boost::filesystem::ifstream file(path);
    std::stringstream file_contents;
    file_contents << file.rdbuf();

    // Set the response status to OK (200) and set the body of the response to the contents of the requested file
    reply_->result(http::status::ok);
    reply_->set(http::field::content_type, "text/plain");
    reply_->body() = file_contents.str();
    reply_->set(http::field::content_length, std::to_string(reply_->body().size()));
}
