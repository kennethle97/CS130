#include "../include/request_handler.h"
#include "../include/request_handler_dispatcher.h"
#include "../include/request_handler/request_handler_echo.h"
#include "../include/request_handler/request_handler_static.h"

Request_Handler_Dispatcher::Request_Handler_Dispatcher(const NginxConfig & config){
    parse_config_handlers(config);
}

void Request_Handler_Dispatcher::parse_config_handlers(const NginxConfig& config) {
    for (const auto& statement : config.statements_) {
        if (statement->child_block_.get() != nullptr) {
            for (const auto& child_statement : statement->child_block_->statements_) {
                /*If child_statement includes location directive and has a total length of two tokens we create begin to create a request_handler.*/
                if (child_statement->tokens_[0] == "location" && child_statement->tokens_.size() == 2) {
                    path_uri path = child_statement->tokens_[1];
                    if (map_handlers.find(path) != map_handlers.end()) {
                        std::cerr << "Duplicate handler for path: " << path << std::endl;
                        continue;
                    }
                    std::shared_ptr<Request_Handler> handler;
                    if (child_statement->child_block_.get()!= nullptr) {
                        // Handler for serving static files
                        path_uri root;
                        for (const auto& child_block_statement : child_statement->child_block_->statements_) {
                            /*If statement include root directive we create a static request handler with the given path*/
                            if (child_block_statement->tokens_[0] == "root" && child_block_statement->tokens_.size() == 2) {
                                root = child_block_statement->tokens_[1];
                                break;
                            }
                            /*If no directive is included we assign root to / */
                            else{
                                root = "/";
                            }
                        
                        }
                        handler = std::make_shared<Request_Handler_Static>(root, path);
                    } else {
                        // Else we create an Echo_Request_handler for the given path.
                        handler = std::make_shared<Request_Handler_Echo>();
                    }
                    map_handlers[path] = handler;
                }
            }
        }
    }
}

std::shared_ptr<Request_Handler> Request_Handler_Dispatcher::get_request_handler(const request &http_request) const{
    // Extract URI from request
    path_uri uri = http_request.uri;
    if (uri.empty() || uri[0] != '/') {
        // Invalid URI, Path URI is completely empty
        return nullptr;
    }
    // Remove trailing slashes
    while (uri.length() > 1 && uri.back() == '/') {
        uri.pop_back();
    }
    auto it = map_handlers.find(uri);
    //Start off with whole uri and popback characters until we find a match in map_handlers for matching path.
    if (uri.length() != 1) {
        do {
            it = map_handlers.find(uri);
            if (it != map_handlers.end()) {
                break;
            }
            uri.pop_back();
        } while (uri.length() > 1);
    }
    // Search for matching handler
    if (it == map_handlers.end()) {
        // No matching handler found
        return nullptr;
    }
    // Return matching handler
    return it->second;
}


