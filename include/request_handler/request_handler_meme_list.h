#ifndef REQUEST_HANDLER_MEME_LIST_H
#define REQUEST_HANDLER_MEME_LIST_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "../request_handler.h"
#include "../request_handler_dispatcher.h"
#include "../nlohmann/json.hpp"
#include <string>
#include <tuple>
#include <vector>
#include <regex>
#include <fstream>
#include <mutex>




class Request_Handler_Meme_List: public Request_Handler {
    public:
        Request_Handler_Meme_List(const path_uri& data_path_, const path_uri& location_, const path_uri& url_);
        void handle_request(const request &http_request, reply *http_reply)noexcept override;
        
        path_uri get_data_path() const {return data_path_;}
    private:

        path_uri data_path_;    // Root path to serve files from
        path_uri location_;  // Location to match with root directory
        path_uri url_;   // Full path from http request

        int num_memes;

        struct meme_data {
            int id;
            int num_likes;
            std::string file_path;
        };

        void write_base_http(reply *http_reply);
        void write_response(nlohmann::json json_body, reply *http_reply);
        void write_not_found_meme_response(reply *http_reply);
        void write_bad_request(reply *http_reply);

        std::string use_configured_root(reply *http_reply);
        void init_meme_map();
        void sort_meme_likes();
        void sort_meme_time();
        

        bool check_if_exists(std::string meme_name,reply *http_reply);
        bool is_valid_ip_address(const std::string ip_address);
        bool is_valid_json(std::string json_conents);

        meme_data get_meme_data(std::string meme_name);

        std::string get_entity(std::string path);
        void update_entity(const std::string& path,const nlohmann::json& json_contents);
        std::string convert_name_from_url(std::string meme_name);
        
        void handle_get(std::string filename,reply *http_reply);
        void handle_post(std::string filename,std::string ip_address,reply *http_reply);


        //Map is initalized where the string is the key and the tuple has the format <id,filename,number_of_likes>
        std::unordered_map<std::string, meme_data> meme_map;
        std::vector<std::pair<int,std::string>> ordered_likes;
        std::vector<std::pair<int,std::string>> ordered_time;
        
};

#endif