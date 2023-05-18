#include "request_handler/request_handler_static.h"
#include "request_handler/static_handler_factory.h"
#include "logger.h"

Static_Handler_Factory::Static_Handler_Factory(NginxConfig config) {
    this->config = config;
}

Request_Handler_Static* Static_Handler_Factory::create(const std::string& location_, const std::string& url_) {
    std::string root_ = parse_config(this->config, location_);
    if (root_ == "#") {
        return nullptr;
    }
    return new Request_Handler_Static(root_, location_, url_);
}


// gets root for static handler
std::string Static_Handler_Factory::parse_config(NginxConfig config, std::string location) {
    for (const auto &statement : config.statements_) {
        if (statement->child_block_.get() != nullptr) {
            if (statement->tokens_.size() == 3 && statement->tokens_[0] == "location") {
                path_uri path = statement->tokens_[1];
                path_handler_name handler_name = statement->tokens_[2];
                while (path.length() > 1 && path.back() == '/') {
                    path.pop_back();
                }

                if (handler_name == STATIC_HANDLER && statement->child_block_.get() != nullptr && location == path) {
                    ServerLogger *server_logger = ServerLogger::get_server_logger();
                    path_uri root = "\0"; 
                    for (const auto &child_statement : statement->child_block_->statements_) {
                        if (child_statement->tokens_.size() == 2 && child_statement->tokens_[0] == "root") {
                            root = child_statement->tokens_[1];
                            while (root.length() > 1 && root.back() == '/') {
                                root.pop_back();
                            }
                            server_logger->log_trace("Parsed StaticHandler root " + root + " for location " + location);
                            return root;
                        }
                    }

                    
                }
            }
        }
    }
    return "#"; // set root to #, which is illegal directory character, if root doesn't exist
}