#include "request_handler/request_handler_static.h"
#include "request_handler/static_handler_factory.h"

Static_Handler_Factory::Static_Handler_Factory(NginxConfig config) {
    this->config = config;
}

Request_Handler_Static* Static_Handler_Factory::create(const std::string& location_, const std::string& url_) {
    std::string root_ = parse_config_root(this->config, location_);
    if (root_ == "#") {
        return nullptr;
    }
    return new Request_Handler_Static(root_, location_, url_);
}


// note: doesnt actually work because it doesnt look for longest matching prefix
// likely will have to create a map of sorts to work this out
// static map?
std::string Static_Handler_Factory::parse_config_root(NginxConfig config, std::string location) {
    for (const auto &statement : config.statements_) {
        if (statement->child_block_.get() != nullptr) {
            if (statement->tokens_.size() == 3 && statement->tokens_[0] == "location") {
                path_uri path = statement->tokens_[1];
                path_handler_name handler_name = statement->tokens_[2];
                while (path.back() == '/') {
                    path.pop_back();
                }

                if (handler_name == STATIC_HANDLER && statement->child_block_.get() != nullptr && location == path) {
                    // add '/' to end of path if it doesn't have one
                    // if (path.back() != '/') {
                    //     path += '/';
                    // }

                    path_uri root = "#"; // set root to #, which is illegal directory character to see if it is set later
                    for (const auto &child_statement : statement->child_block_->statements_) {
                        if (child_statement->tokens_.size() == 2 && child_statement->tokens_[0] == "root") {
                            root = child_statement->tokens_[1];
                            // add '/' to end of root if it doesn't have one
                            if (root.back() != '/') {
                                root += '/';
                            }
                            return root;
                        }
                    }

                    
                }
            }
        }
    }
    return "#";
}