
#include "../../include/request_handler/request_handler_meme.h"
#include "../../include/request_handler/meme_handler_factory.h"
#include "logger.h"

Meme_Handler_Factory::Meme_Handler_Factory(NginxConfig config) {
    this->config = config;
    meme_locks = std::make_shared<std::unordered_map<std::string,std::unique_lock<std::mutex>>> ();
    meme_map = std::make_shared<std::unordered_map<std::string,Request_Handler_Meme::meme_data>> ();
    meme_map_lock = std::make_shared<std::shared_mutex>();

}   

Request_Handler_Meme* Meme_Handler_Factory::create(const path_uri& location_, const path_uri& url_) {
    std::string data_path = parse_config(this->config, location_);
    if (data_path == "#") {
        return nullptr;
    }
    return new Request_Handler_Meme(data_path, location_, url_,meme_locks,meme_map,meme_map_lock);
}

std::string Meme_Handler_Factory::parse_config(NginxConfig config, std::string location) {
    for (const auto &statement : config.statements_) {
        if (statement->child_block_.get() != nullptr) {
            if (statement->tokens_.size() == 3 && statement->tokens_[0] == "location") {
                path_uri path = statement->tokens_[1];
                path_handler_name handler_name = statement->tokens_[2];
                while (path.length() > 1 && path.back() == '/') {
                    path.pop_back();
                }

                if (handler_name == MEME_HANDLER && statement->child_block_.get() != nullptr && location == path) {
                    ServerLogger *server_logger = ServerLogger::get_server_logger();
                    path_uri data_path = "\0"; 
                    for (const auto &child_statement : statement->child_block_->statements_) {
                        if (child_statement->tokens_.size() == 2 && child_statement->tokens_[0] == "data_path") {
                            data_path = child_statement->tokens_[1];
                            while (data_path.length() > 1 && data_path.back() == '/') {
                                data_path.pop_back();
                            }
                            server_logger->log_trace("Parsed Meme Handler data_path " + data_path + " for location " + location);
                            return data_path;
                        }
                    }
                }
            }
        }
    }
    return "#"; // set data_path to #, which is illegal directory character, if data_path doesn't exist
}