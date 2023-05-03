
#include "../../include/request_handler/request_handler_static.h"

RequestHandlerStatic::RequestHandlerStatic(const path_uri& root_, const path_uri& prefix_)
    : root(root_), prefix(prefix_) {}

void RequestHandlerStatic::handle_request(const request &request_, response *response) noexcept {
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


    boost::filesystem::path boost_path(uri);
    std::string extension = boost_path.extension().string();
    
    // Set the response status to OK (200) and set the body of the response to the contents of the requested file
    http_reply->result(http::status::ok);
    http_reply->set(http::field::content_type, "text/plain");
    http_reply->body() = file_contents.str();
    http_reply->set(http::field::content_length, std::to_string(reply_->body().size()));
}
