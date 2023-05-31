#ifndef CRUD_HANDLER_FACTORY_H
#define CRUD_HANDLER_FACTORY_H 

#include "request_handler/request_handler_crud.h"
#include "config_parser.h"

class Crud_Handler_Factory : public Request_Handler_Factory {
    public:
        Crud_Handler_Factory(NginxConfig config);
        Request_Handler_Crud* create(const path_uri& location, const path_uri& url);
        std::shared_ptr<std::map<std::string, std::map<int, std::string>>> sub_directory_map;
        
    private:
        NginxConfig config;
        std::string parse_config(NginxConfig config, std::string location);
};

#endif