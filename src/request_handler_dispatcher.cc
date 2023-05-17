#include "../include/request_handler.h"
#include "../include/request_handler_dispatcher.h"
#include "../include/request_handler/request_handler_echo.h"
#include "../include/request_handler/request_handler_static.h"
#include "../include/request_handler/static_handler_factory.h"
#include "../include/request_handler/echo_handler_factory.h"
#include "../include/request_handler/request_404_handler_factory.h"
#include "../include/logger.h"

/*Constructor calls parse_config_handlers to initialize the map of handlers*/

Request_Handler_Dispatcher::Request_Handler_Dispatcher(const NginxConfig & config) {
    parse_config_handlers(config);
}
/*
The function parse_config_handlers takes in a NginxConfig type object and parses it to create the list of request handlers specificed
in the config file by checking to see if there contains a statement with a "location" token as its first argument and sets the second
token as the path. If the statement has its own child block then it looks for a root directive and to create a static request_handler.
All the request_handlers are paired with their respective path variable name and are set in the member variable map_handlers()

*/
void Request_Handler_Dispatcher::parse_config_handlers(const NginxConfig& config) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();

    for (const auto &statement : config.statements_) {
        if (statement->child_block_.get() != nullptr) {
            if (statement->tokens_.size() == 3 && statement->tokens_[0] == "location") {
                path_uri path = statement->tokens_[1];
                path_handler_name handler_name = statement->tokens_[2];
                while (path.back() == '/' && path.length() > 1) {
                    path.pop_back();
                }

                if (map_handlers.find(path) != map_handlers.end()) {
                    server_logger->log_error("Another handler exists at path: " + path);
                    continue;
                } else {
                    server_logger->log_trace("Parsed handler " + handler_name + " for path: " + path);
                }

                std::shared_ptr<Request_Handler_Factory> handler_factory;
                if (handler_name == STATIC_HANDLER && statement->child_block_.get() != nullptr) {
                    // add '/' to end of path if it doesn't have one
                    // if (path.back() != '/') {
                    //     path += '/';
                    // }

                    path_uri root = "#"; // set root to #, which is illegal directory character to see if it is set later
                    for (const auto &child_statement : statement->child_block_->statements_) {
                        if (child_statement->tokens_.size() == 2 && child_statement->tokens_[0] == "root") {
                            root = child_statement->tokens_[1];
                            break;
                        }
                    }

                    if (root != "#") { // if root is not #, then it was set in the config file
                        server_logger->log_trace("Dispatch static handler at path: " + path + " with root: " + root);
                        // add '/' to end of root if it doesn't have one
                        if (root.back() != '/') {
                            root += '/';
                        }
                        handler_factory = std::make_shared<Static_Handler_Factory>(config);
                    } else {
                        server_logger->log_error("No root folder specified for static handler at path: " + path);
                    }
                } else if (handler_name == ECHO_HANDLER) {
                    if (statement->child_block_->statements_.size() > 0) {
                        server_logger->log_warning("EchoHandler block has statements, which will be ignored");
                    }
                    server_logger->log_trace("Dispatch echo handler factory at path: " + path);
                    handler_factory = std::make_shared<Echo_Handler_Factory>(path);

                } else {
                    server_logger->log_debug("Error Handler init");
                    handler_factory = std::make_shared<Request_404_Handler_Factory>();
                }
                map_handlers[path] = handler_factory;
            }
        }
    }
}

/*
The function get_request_handler_factory(const request &http_request) takes in a http_request and returns the shared_ptr for the request_handler_factory
if it exists in the dispatcher's member variable map_handlers. If it doesn't find a match it returns a nullptr of which will result in a reply
of a bad request.
*/
std::shared_ptr<Request_Handler_Factory> Request_Handler_Dispatcher::get_request_handler_factory(const request &http_request) const {
    std::string location = match(http_request);

    // Either no matching handler was found or Invalid URI
    if (location == "") {
        return nullptr;
    } else {
        auto it = map_handlers.find(location);
        return it->second;
    }
}


std::string Request_Handler_Dispatcher::match(const request &http_request) const {
    // Extract URI from request
    path_uri uri = http_request.uri;
    if (uri.empty() || uri[0] != '/') {
        // Invalid URI, Path URI is completely empty
        return "";
    }
    // Remove trailing slashes
    while (uri.length() > 1 && uri.back() == '/') {
        uri.pop_back();
    }
    auto it = map_handlers.find(uri);

    char suffix_char = '\0';
    //Start off with whole uri and popback characters until we find a match in map_handlers for matching path.
    if (uri.length() != 1) {
        do {
            it = map_handlers.find(uri);
            if (it != map_handlers.end() && (suffix_char == '\0' || suffix_char == '/')) {
                break;
            }
            suffix_char = uri.back();
            uri.pop_back();
        } while (uri.length() > 1);
    }
    // Search for matching handler
    if (it == map_handlers.end()) {
        // No matching handler found
        return "";
    }
    // Return matching handler
    return it->first;
}
