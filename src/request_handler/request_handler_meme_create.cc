#include "../../include/request_handler/request_handler_meme_create.h"
#include "../../include/http/mime_types.hpp"
#include "logger.h"
#include <cstdio>
#include <string>
#include <nlohmann/json.hpp>

Request_Handler_Meme_Create::Request_Handler_Meme_Create(const path_uri& root_, const path_uri& location_, const path_uri& url_) 
    : root(root_), location(location_), url(url_) {}

std::string Request_Handler_Meme_Create::use_configured_root(reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();

    std::string uri = this->url;
    // remove any trailing backslashes from uri
    while (uri.back() == '/') {
        uri.pop_back();
    }
    std::size_t location_pos = uri.find(this->location);
    if (location_pos != std::string::npos) {
        uri.replace(location_pos, this->location.length(), root);
    } else {
        http_reply->result(boost::beast::http::status::not_found);
        const char not_found[] =
                        "<html>"
                        "<head><title>Not Found</title></head>"
                        "<body><h1>404 Not Found</h1></body>"
                        "</html>\n";
        http_reply->body() = not_found;
        http_reply->content_length(http_reply->body().size());
        http_reply->set(boost::beast::http::field::content_type, "text/html");
        server_logger->log_trace("-- create meme file location not found");
        server_logger->log_info("[HandlerMetrics] Meme_create_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
        return NULL;
        }
    return uri;
    }
void Request_Handler_Meme_Create::write_invalid_json_response(reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    http_reply->result(boost::beast::http::status::unsupported_media_type);                   
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    http_reply->set(boost::beast::http::field::content_disposition, "inline");
    http_reply->body() =  "Unsupported Media Type: The provided file type is not supported.\n";
    http_reply->content_length(http_reply->body().size());
}
bool Request_Handler_Meme_Create::isRequestValidJson(std::string json_data)
{
    try{   // Parse the body as JSON
        nlohmann::json bodyJson = nlohmann::json::parse(json_data);
        // Check if parsing was successful
        return true;
    }
    catch (const nlohmann::json::parse_error& e){
        return false;
    }
}
void Request_Handler_Meme_Create::create_meme(std::string path, std::string json_data){
    //create the path with the filename already passed in with the extension,.json
    boost::filesystem::path file_path(path);
    std::ofstream file(file_path);
    file << json_data;
    file.close();
}

std::string Request_Handler_Meme_Create::update_field_id(std::string rootpath){
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    //meme_id_tracker.txt is the file to track field id
    std::string file_path=rootpath + "/meme_id_tracker.txt";;
    boost::filesystem::path path(file_path);
    std::ifstream file (path);
    std::string field_id;

    if(file.is_open()){     
        while(file){
            file >> field_id;
        }          
    }
    else{
        server_logger->log_warning("-- meme_id_tracker file not found");
    }
    file.close();
    remove(path);
    
    int count= std::stoi(field_id);
    //update field id
    count+=1;
    field_id= std::to_string(count);
    std::ofstream newfile(path);
    newfile << field_id;
    newfile.close();

    return field_id;
};

void Request_Handler_Meme_Create::handle_request(const request &http_request, reply *http_reply) noexcept {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    
    // Configure the rootpath path with the configured root path
    std::string rootpath = use_configured_root(http_reply);

    // Get method of request to call appropiate function
    std::string method = http_request.method_string().to_string();

    if (method == "POST")
    {    // Handle POST request
        std::string json_data = http_request.body();
        std::string field_id;
        std::string file_path;
        std::string file_name;

        bool is_json = isRequestValidJson(json_data);

        if(!is_json){
        //LOG invalid file type. Not a json file type.
            server_logger->log_trace("-- Meme Create ivalid file json type");
            write_invalid_json_response(http_reply);
            return;
        }
       
        field_id=update_field_id(rootpath);
        //example file name: 2.json
        file_name = field_id + ".json";
        file_path = rootpath + "/" + file_name;

        create_meme(file_path, json_data);
        
        http_reply->set(boost::beast::http::field::content_type, "text/plain");
	    http_reply->set(boost::beast::http::field::content_disposition, "inline");
	    http_reply->body() = std::string("{file name: ") + file_name + "}\n";
        http_reply->content_length(http_reply->body().size());  
        server_logger->log_trace("--  meme create valid ");
        server_logger->log_info("[HandlerMetrics] Meme_create_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));   

    }
    else{
        http_reply->result(boost::beast::http::status::bad_request);
        http_reply->set(boost::beast::http::field::content_type, "text/plain");
        http_reply->set(boost::beast::http::field::content_disposition, "inline");
        const char bad_request[] =
                            "<html>"
                            "<head><title>Bad Request</title></head>"
                            "<body><h1>400 Bad Request</h1></body>"
                            "</html>\n";
        http_reply->body() = bad_request;
        http_reply->content_length(http_reply->body().size());
        server_logger->log_trace("-- meme create ivalid api request");
        server_logger->log_info("[HandlerMetrics] Meme_Create_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
    }
        
}