#include "../include/request_handler.h"
#include "../include/request_handler_dispatcher.h"
#include "../include/request_handler/request_handler_echo.h"
#include "../include/request_handler/request_handler_static.h"

Request_Handler_Dispatcher::request_handler_dispatcher(const NginxConfig & config){
    parse_config_handlers(config);
}

void Request_Handler_Dispatcher::parse_config_handlers(const NginxConfig& config) {
    for (const auto& statement : config.statements_) {
        if (statement->tokens_[0] == "location" && statement->tokens_.size() == 2) {
            PathUri path_uri = statement->tokens_[1];
            if (map_handlers.find(path_uri) != map_handlers.end()) {
                std::cerr << "Duplicate handler for path: " << path_uri << std::endl;
                continue;
            }
            std::shared_ptr<Request_Handler> handler;
            if (statement->child_block_) {
                // Handler for serving static files
                PathUri root_dir;
                for (const auto& child_statement : statement->child_block_->statements_) {
                    if (child_statement->tokens_[0] == "root") {
                        root_dir = child_statement->tokens_[1];
                        break;
                    }
                }
                handler = std::make_shared<Request_Handler_Static>(root_dir, path_uri);
            } else {
                // Handler for other types of requests
                handler = std::make_shared<Request_Handler_Echo>();
            }
            map_handlers[path_uri] = handler;
        }
    }
}

std::shared_ptr<const Request_Handler> Request_Handler_Dispatcher::get_request_handler(request &request) const{
    // Extract URI from request
    PathUri uri = request.target().to_string();
    if (uri.empty() || uri[0] != '/') {
        // Invalid URI
        return nullptr;
    }
    // Remove trailing slashes
    while (uri.length() > 1 && uri.back() == '/') {
        uri.pop_back();
    }

    // Search for matching handler
    auto it = map_handlers.find(uri);
    if (it == map_handlers.end()) {
        // No matching handler found
        return nullptr;
    }

    // Return matching handler
    return it->second;
}


