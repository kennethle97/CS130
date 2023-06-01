/*Define top level class for request_handlers to be able to add more in the future if necessary*/

#ifndef REQUEST_HANDLER_CRUD_H
#define REQUEST_HANDLER_CRUD_H
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "../request_handler.h"
#include "../request_handler_dispatcher.h"
#include "../nlohmann/json.hpp"

class Request_Handler_Crud: public Request_Handler {
    public:
        Request_Handler_Crud(const path_uri& data_path_, const path_uri& location_, const path_uri& url_,std::shared_ptr<std::map<std::string, std::map<int, std::string>>> entity_map);
        void handle_request(const request &http_request, reply *http_reply)noexcept override;
        
        path_uri get_data_path() const {return data_path_;}
    private:

        path_uri data_path_;    // Root path to serve files from
        path_uri location_;  // Location to match with root directory
        path_uri url_;   // Full path from http request

        void write_base_http(reply *http_reply);
        void write_response(std::string content_body, reply *http_reply);
        void write_not_found_json_response(reply *http_reply);
        void write_invalid_json_response(reply *http_reply);
        void write_file_exists_response(int file_id,reply *http_reply);
        bool isRequestValidJson(std::string json_data);
        
        std::string use_configured_root(reply *http_reply);
        void init_entity_map();
        bool is_ID(std::string path_extension);
        void rec_create_map(const boost::filesystem::path& dir_path);
        int get_next_id(std::string path);

        void create_subdirectory(std::string path);
        void create_entity(std::string path,std::string json_payload);
        void delete_entity(std::string path, std::string sub_directory, int file_id);
        void delete_entity_folder(std::string path);

        bool check_if_exists(std::string sub_directory, int file_id,reply *http_reply);
        bool check_if_id(std::string path_extension);
        bool check_if_possible_dir(std::string path_extension);
        std::string get_entity(std::string path);
        
        void handle_post(std::string filename,std::string json_payload,reply *http_reply);
        void handle_put(std::string filename, std::string json_payload,reply *http_reply);
        void handle_get(std::string filename,reply *http_reply);
        void handle_delete(std::string filename,reply *http_reply);
        
        std::shared_ptr<std::map<std::string, std::map<int, std::string>>> sub_directory_map;
};

#endif