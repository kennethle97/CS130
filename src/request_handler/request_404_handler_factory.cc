#include "request_handler/request_handler_404.h"
#include "request_handler/request_404_handler_factory.h"
#include "request_handler_dispatcher.h"
#include "logger.h"

Request_404_Handler_Factory::Request_404_Handler_Factory(NginxConfig config) {
    this->config = config;
}

Request_Handler_404* Request_404_Handler_Factory::create(const std::string& location_, const std::string& url) { 
    bool parse_status = parse_config(this->config, location_);
    if (!parse_status) {
        return nullptr;
    }
    return new Request_Handler_404(location_);
}

bool Request_404_Handler_Factory::parse_config(NginxConfig config, std::string location) {
    for (const auto &statement : config.statements_) {
        if (statement->child_block_.get() != nullptr) {
            if (statement->tokens_.size() == 3 && statement->tokens_[0] == "location") {
                path_uri path = statement->tokens_[1];
                path_handler_name handler_name = statement->tokens_[2];
                while (path.length() > 1 && path.back() == '/') {
                    path.pop_back();
                }

                if (handler_name != STATIC_HANDLER && handler_name != ECHO_HANDLER && location == path) {
                    ServerLogger *server_logger = ServerLogger::get_server_logger();
                    if (statement->child_block_->statements_.size() > 0) {
                        server_logger->log_warning("404 Handler block has statements, which will be ignored");
                    }
                    return true; // we found a good 404 handler declaration, so return true
                } 
            }
        }
    }
    return false;
}
