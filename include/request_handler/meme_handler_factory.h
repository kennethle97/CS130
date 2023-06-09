#ifndef MEME_HANDLER_FACTORY_H
#define MEME_HANDLER_FACTORY_H 

#include "request_handler/request_handler_meme.h"
#include "config_parser.h"

class Meme_Handler_Factory : public Request_Handler_Factory {
    public:
        Meme_Handler_Factory(NginxConfig config);
        Request_Handler_Meme* create(const path_uri& location, const path_uri& url);

        std::shared_ptr<std::unordered_map<std::string,std::unique_lock<std::mutex>>> meme_locks;
        std::shared_ptr<std::unordered_map<std::string,Request_Handler_Meme::meme_data>> meme_map;
        std::shared_ptr<std::shared_mutex> meme_map_lock;

    private:
        NginxConfig config;
        std::string parse_config(NginxConfig config, std::string location);
};

#endif