#ifndef REQUEST_HANDLER_MEME_LIST_H
#define REQUEST_HANDLER_MEME_LIST_H
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "../request_handler.h"
#include "../request_handler_dispatcher.h"
#include "../nlohmann/json.hpp"


class Request_Handler_Meme_List: public Request_Handler {
    public:
        Request_Handler_Meme_List(const path_uri& data_path_, const path_uri& location_, const path_uri& url_);
        void handle_request(const request &http_request, reply *http_reply)noexcept override;
        
        path_uri get_data_path() const {return data_path_;}
    private:

        path_uri data_path_;    // Root path to serve files from
        path_uri location_;  // Location to match with root directory
        path_uri url_;   // Full path from http request

        void write_base_http(reply *http_reply);
        void write_response(std::string content_body, reply *http_reply);
        void write_not_found_meme_response(reply *http_reply);
        
        std::string use_configured_root(reply *http_reply);
        void init_entity_map();
        void init_likes_map();
        void rec_create_map();
        
        void parse_json_likes(const boost::filesystem::path& json_path);
        void sort_meme_likes();

        bool check_if_exists(std::string meme_name,reply *http_reply);
        
        std::string get_entity(std::string path);
        
        void handle_get(std::string filename,reply *http_reply);

        std::map<std::string, std::pair<int,std::string> meme_map;
        std::map<std::string, int> map_likes;
        std::vector<std::pair<std::string,int>> ordered_likes;
        

        
};

#endif