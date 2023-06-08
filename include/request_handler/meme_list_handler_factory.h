#ifndef MEME_LIST_HANDLER_FACTORY_H
#define MEME_LIST_HANDLER_FACTORY_H 

#include "request_handler/request_handler_meme_list.h"
#include "config_parser.h"

class Meme_List_Handler_Factory : public Request_Handler_Factory {
    public:
        Meme_List_Handler_Factory(NginxConfig config);
        Request_Handler_Meme_List* create(const std::string& location, const std::string& url);

    private:
        NginxConfig config;
        std::string parse_config(NginxConfig config, std::string location);
};

#endif