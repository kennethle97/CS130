#ifndef MEME_CREATE_HANDLER_FACTORY_H
#define MEME_CREATE_HANDLER_FACTORY_H 

#include "request_handler/request_handler_meme_create.h"
#include "config_parser.h"

class Meme_Create_Handler_Factory : public Request_Handler_Factory {
    public:
        Meme_Create_Handler_Factory(NginxConfig config);
        Request_Handler_Meme_Create* create(const path_uri& location, const path_uri& url);
       
    private:
        NginxConfig config;
        std::string parse_config(NginxConfig config, std::string location);
};

#endif