
#include "../../include/request_handler/request_handler_crud.h"
#include "../../include/http/mime_types.hpp"
#include "logger.h"
#include <string>

Request_Handler_Crud::Request_Handler_Crud(const path_uri& data_path_, const path_uri& location_, const path_uri& url_,std::shared_ptr<std::map<std::string, std::map<int, std::string>>> entity_map) {
    this->data_path_ = data_path_;
    this->location_ = location_;
    this->url_ = url_;
    this->sub_directory_map = entity_map;
    init_entity_map();
}

void Request_Handler_Crud::init_entity_map(){
    if(sub_directory_map->empty()){
        const boost::filesystem::path& root_path(this->data_path_);
        if(!boost::filesystem::exists(root_path)){
            boost::filesystem::create_directory(root_path);
        }
        else{
            rec_create_map(root_path);
        }
    }
}

/*rec_create_map recursively intializes the map of entities for a given root directory*/
void Request_Handler_Crud::rec_create_map(const boost::filesystem::path& dir_path){
    for (auto& entry : boost::filesystem::directory_iterator(dir_path)) {
        
        auto& path = entry.path();
        if (boost::filesystem::is_directory(path)) {
            // Recursively traverse into subdirectories until we reach a file
            rec_create_map(path);
        } else {
            auto& filename = path.filename().string();
            auto& sub_path = path.string();
            std::string new_path  = sub_path.substr(0,sub_path.find_last_of("/") + 1);
            std::string id = filename.substr(0,filename.find_last_of("."));
            int file_id = std::stoi(id);
            (*sub_directory_map)[new_path][file_id] = filename;
            
            //For some reason this line below is necessary for the file_name to resolve correctly in the map.
            auto test = (*sub_directory_map)[new_path][file_id];
        }
    }
}

std::string Request_Handler_Crud::use_configured_root(reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();

    std::string uri = this->url_;

    // remove any trailing backslashes from uri
    while (uri.back() == '/') {
        uri.pop_back();
    }
    
    std::size_t location_pos = uri.find(this->location_);
    if (location_pos != std::string::npos) {
        uri.replace(location_pos, this->location_.length(), data_path_);
    } else {
        //*******stock reply************
        http_reply->result(boost::beast::http::status::not_found);
        const char not_found[] =
                        "<html>"
                        "<head><title>Not Found</title></head>"
                        "<body><h1>404 Not Found</h1></body>"
                        "</html>\n";
        http_reply->body() = not_found;
        http_reply->content_length(http_reply->body().size());
        http_reply->set(boost::beast::http::field::content_type, "text/html");
        //*******stock reply************
        server_logger->log_trace("-- crud file location not found");
        server_logger->log_info("[HandlerMetrics] Crud_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
        return NULL;
        }
    return uri;
    }
bool Request_Handler_Crud::check_if_exists(std::string sub_directory, int file_id,reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    //If the sub-directory already exists
    if(boost::filesystem::exists(sub_directory)){

        if(file_id != -1){
            auto entity_map = (*sub_directory_map)[sub_directory];
            auto it = entity_map.find(file_id);

            if(it == entity_map.end()){
            // Case where the file does not exist at the url path.
                server_logger->log_trace("-- crud file not found");
                write_not_found_json_response(http_reply);
                return false;
            }
        }
        else if(file_id == -1){
            // Case where no id was found: error
            server_logger->log_trace("-- crud file id not found");
            write_not_found_json_response(http_reply);
            return false;
        }
    }
    // subdirectory doesn't exist, this means that existing file can't be found via the subdirectory provided
    else if(!boost::filesystem::exists(sub_directory)){
        // Case where no id was found: error 
        server_logger->log_trace("-- crud file id not found");
        write_not_found_json_response(http_reply);
        return false;
    }
    return true;
}

bool Request_Handler_Crud::check_if_id(std::string path_extension) {
    return (!path_extension.empty() && is_ID(path_extension));
}

bool Request_Handler_Crud::check_if_possible_dir(std::string path_extension) {
    return (!path_extension.empty() && !is_ID(path_extension));
}

bool Request_Handler_Crud::is_ID(std::string path_extension){
    for(char c: path_extension){
        if(!std::isdigit(c)){
            return false;
        }
    }
    return true;
}
bool Request_Handler_Crud::isRequestValidJson(std::string json_data)
{
    try
    {
        // Parse the body as JSON
        nlohmann::json bodyJson = nlohmann::json::parse(json_data);
        // Check if parsing was successful
        return true;
    }
    catch (const nlohmann::json::parse_error& e)
    {
        return false;
    }
}

int Request_Handler_Crud::get_next_id(std::string path){
    auto entity_map = (*sub_directory_map)[path];
    int next_available_id = 1 ;
    while(entity_map.count(next_available_id) > 0){
        next_available_id++;
    }
    return next_available_id;
}

void Request_Handler_Crud::create_subdirectory(std::string path){
    //Create a new map pair value for the new sub_directory_path
    sub_directory_map->emplace(path, std::map<int,std::string>());
    boost::filesystem::create_directories(path);

}

void Request_Handler_Crud::create_entity(std::string path,std::string json_data){
    //create the path with the filename already passed in with the extension,.json
    boost::filesystem::path file_path(path);
    
    std::ofstream file(file_path);
    file << json_data;
    file.close();
}

std::string Request_Handler_Crud::get_entity(std::string path) {
    // retrieve the contents of the file specified by path
    boost::filesystem::path file_path(path);

    std::ifstream file(file_path);
    std::stringstream content;
    content << file.rdbuf();
    file.close();
    
    return content.str();
}

// deletes an entity id
void Request_Handler_Crud::delete_entity(std::string path, std::string sub_directory, int file_id) {
    //create the path with the filename already passed in with the extension,.json
    boost::filesystem::path file_path(path);

    // remove file from entity map
    (*sub_directory_map)[sub_directory].erase(file_id);

    // remove file from filesystem
    remove(file_path);
}

// deletes an entire entity directory
void Request_Handler_Crud::delete_entity_folder(std::string path) {
    //create the path with the filename already passed in with the extension,.json
    boost::filesystem::path file_path(path);

    // remove file from entity map
    (*sub_directory_map).erase(path);

    // remove file from filesystem
    boost::filesystem::remove_all(file_path);
}
void Request_Handler_Crud::write_base_http(reply *http_reply){
    http_reply->body().clear();
    http_reply->keep_alive(false);
    http_reply->result(boost::beast::http::status::ok);
    http_reply->set(boost::beast::http::field::server,"Beast");

}
void Request_Handler_Crud::write_response(std::string content_body,reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
	http_reply->set(boost::beast::http::field::content_type, "text/plain");
	http_reply->set(boost::beast::http::field::content_disposition, "inline");
	http_reply->body() = content_body + "\n";
    http_reply->content_length(http_reply->body().size());
    server_logger->log_info("[HandlerMetrics] Crud_handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));    
}

void Request_Handler_Crud::write_not_found_json_response(reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    http_reply->result(boost::beast::http::status::not_found);
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    http_reply->set(boost::beast::http::field::content_disposition, "inline");
    http_reply->body() = "Could not resolve ID -- ID not found.\n";
    http_reply->content_length(http_reply->body().size());
    server_logger->log_info("[HandlerMetrics] Crud_handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
}

void Request_Handler_Crud::write_invalid_json_response(reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    http_reply->result(boost::beast::http::status::unsupported_media_type);                   
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    http_reply->set(boost::beast::http::field::content_disposition, "inline");
    http_reply->body() =  "Unsupported Media Type: The provided file type is not supported.\n";
    http_reply->content_length(http_reply->body().size());
}

void Request_Handler_Crud::write_file_exists_response(int file_id,reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    http_reply->result(boost::beast::http::status::conflict);                   
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    http_reply->set(boost::beast::http::field::content_disposition, "inline");
    http_reply->body() = std::string("File already exists at path with ID: {id:")+ std::to_string(file_id) +std::string("}. To update, make a PUT request.\n");
    http_reply->content_length(http_reply->body().size());
    server_logger->log_info("[HandlerMetrics] Crud_handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));

}

void Request_Handler_Crud::handle_post(std::string request_filename,std::string json_data,reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    write_base_http(http_reply);


    bool is_json = isRequestValidJson(json_data);

    if(!is_json){
        //LOG invalid file type. Not a json file type.
        write_invalid_json_response(http_reply);
        return;
    }

    std::string sub_directory = "";
    int file_id = -1;

    //Takes the file path constructed with the root as before such as /api/subdirectory/id
    std::string path_extension = request_filename.substr(request_filename.find_last_of("/") + 1);
    if(check_if_id(path_extension)){
        //If path has an ending with file_id set the file_id. Otherwise don't.
            sub_directory = request_filename.substr(0,request_filename.find_last_of("/") + 1);
            file_id = std::stoi(path_extension);
            if(file_id == 0){
                //Enforces ID values to be greater than 0 to adhere to Assignment specified API.
                http_reply->result(boost::beast::http::status::conflict);                   
                http_reply->set(boost::beast::http::field::content_type, "text/plain");
                http_reply->set(boost::beast::http::field::content_disposition, "inline");
                http_reply->body() = "Invalid ID. Must be integer greater than 0.\n";
                http_reply->content_length(http_reply->body().size());
                server_logger->log_trace("-- crud create ivalid id request");
                server_logger->log_info("[HandlerMetrics] Crud_handler");
                server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
                return;
                
            } 
        }
    else{
            sub_directory = request_filename += '/';
        }

    std::string entity_id;
    std::string file_name;
    std::string file_path;

    //If the sub-directory already exists
    if(boost::filesystem::exists(sub_directory)){

        if(file_id != -1){
            auto entity_map = (*sub_directory_map)[sub_directory];
            auto it = entity_map.find(file_id);

            if(it != entity_map.end()){
            //Case where the file already exists at the url path.
                write_file_exists_response(file_id,http_reply);
                return;
            }
        }
        else if(file_id == -1){
            file_id = get_next_id(sub_directory);
        }
    }
    //If it doesn't exist then we dynamically create one.
    else if(!boost::filesystem::exists(sub_directory)){
        create_subdirectory(sub_directory);
        if(file_id == -1){
            file_id = get_next_id(sub_directory);
        }
    }
    entity_id = std::to_string(file_id); 
    file_name = entity_id + ".json";
    (*sub_directory_map)[sub_directory][file_id] = file_name;
    //For some reason this line below is necessary for the file_name to resolve correctly in the map.
    auto test = (*sub_directory_map)[sub_directory][file_id];
    file_path = sub_directory + file_name;
    create_entity(file_path,json_data);
   
	http_reply->set(boost::beast::http::field::content_type, "text/plain");
	http_reply->set(boost::beast::http::field::content_disposition, "inline");
	http_reply->body() = std::string("{id: ") + std::to_string(file_id) + "}\n";
    http_reply->content_length(http_reply->body().size());  
    server_logger->log_trace("-- crud create valid ");
    server_logger->log_info("[HandlerMetrics] Crud_handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));   
}

void Request_Handler_Crud::handle_put(std::string request_filename, std::string json_data,reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    write_base_http(http_reply);

    bool is_json = isRequestValidJson(json_data);

    if(!is_json){
        //LOG invalid file type. Not a json file type.
        server_logger->log_trace("-- crud update ivalid file json type");
        write_invalid_json_response(http_reply);
        return;
    }

    std::string sub_directory = "";
    int file_id = -1;

    //Takes the file path constructed with the root as before such as /api/subdirectory/id
    std::string path_extension = request_filename.substr(request_filename.find_last_of("/") + 1);
    if(check_if_id(path_extension)){
        //If path has an ending with file_id set the file_id. Otherwise error, since PUT request expects a file to already exist.
            sub_directory = request_filename.substr(0,request_filename.find_last_of("/") + 1);
            file_id = std::stoi(path_extension); 
        }
    else{
        // Case where no id was found: error for UPDATE request
        server_logger->log_trace("-- crud update file id not found");
        write_not_found_json_response(http_reply);
        return;
    }

    // the check_if_exists function will already set the correct response if doesn't exist, so just return out of function
    if (check_if_exists(sub_directory, file_id,http_reply) == false) {
        return;        
    }
    std::string entity_id;
    std::string file_name;
    std::string file_path;

    entity_id = std::to_string(file_id); 
    file_name = entity_id + ".json";
    file_path = sub_directory + file_name;
    create_entity(file_path,json_data);
    std::string content_body = "Update successful to file with ID: " + std::to_string(file_id);
    server_logger->log_trace("-- crud update valid");
    write_response(content_body,http_reply);
}

void Request_Handler_Crud::handle_get(std::string request_filename,reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    write_base_http(http_reply);
    std::string sub_directory = "";
    int file_id = -1;

    //Takes the file path constructed with the root as before such as /api/subdirectory/id
    std::string path_extension = request_filename.substr(request_filename.find_last_of("/") + 1);
    if(check_if_id(path_extension)) {
        //If path has an ending with file_id set the file_id. Otherwise error, since GET request expects a file to already exist.
        sub_directory = request_filename.substr(0,request_filename.find_last_of("/") + 1);
        file_id = std::stoi(path_extension); 
    }
    // LIST DIRECTORY handling
    else if (check_if_possible_dir(path_extension)) {
        // Path might be for a GET DIRECTORY request, so check if it is a directory
        boost::filesystem::path dir_path(request_filename);
        if (boost::filesystem::is_directory(dir_path)) {
            // GET requests for id store keys in entity map with '/' at end, 
            // but GET requests for folder doesn't have '/' at end, so we add it.
            if (request_filename.back() != '/') {
                request_filename += '/';
            }

            // Add all IDs in entity_map at requested directory to list
            auto entity_map = (*sub_directory_map)[request_filename];
            std::vector<int> ids;
            for (auto it = entity_map.begin(); it != entity_map.end(); ++it) {
                ids.push_back(it->first);
            }

            std::stringstream res;
            if (ids.size() > 0) {
                res << "[";
                for (int i = 0; i < ids.size(); ++i) {
                    // if last element, then don't put comma after id
                    if (i == ids.size() - 1) 
                        res << ids[i];
                    else 
                        res << ids[i] << ", ";
                }
                res << "]";  
            } else {
                res << "[]";
            }
            server_logger->log_trace("-- crud list valid");
            write_response(res.str(),http_reply);
            return;
        } else {
            // not a directory in api folder, so return not found
            server_logger->log_trace("-- crud list file not found");
            write_not_found_json_response(http_reply);
            return;
        }
    }
    else{
        // Case where no id was found: error for GET request
        server_logger->log_trace("-- crud file id not found");
        write_not_found_json_response(http_reply);
        return;
    }

    // the check_if_exists function will already set the correct response if doesn't exist, so just return out of function
    if (check_if_exists(sub_directory, file_id,http_reply) == false) {
        return;        
    }
    std::string entity_id;
    std::string file_name;
    std::string file_path;

    entity_id = std::to_string(file_id); 
    file_name = entity_id + ".json";
    file_path = sub_directory + file_name;
    std::string content = get_entity(file_path); // call get_entity function to retrieve contents of file
    server_logger->log_trace("-- crud retrieve valid");
    write_response(content,http_reply);
}

// DELETE methods are handled here
void Request_Handler_Crud::handle_delete(std::string request_filename,reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    write_base_http(http_reply);
    std::string sub_directory = "";
    int file_id = -1;

    //Takes the file path constructed with the root as before such as /api/subdirectory/id
    std::string path_extension = request_filename.substr(request_filename.find_last_of("/") + 1);

    // First check if the file exists 
    if(check_if_id(path_extension)){
        //If path has an ending with file_id set the file_id. Otherwise error, since DELETE request expects a file to already exist.
        sub_directory = request_filename.substr(0,request_filename.find_last_of("/") + 1);
        file_id = std::stoi(path_extension); 
    }
    // DELETE DIRECTORY handling
    else if (check_if_possible_dir(path_extension)) {
        // Path might be for a DELETE DIRECTORY request, so check if it is a directory
        boost::filesystem::path dir_path(request_filename);
        if (boost::filesystem::is_directory(dir_path)) {
            // GET requests for id store keys in entity map with '/' at end, 
            // but GET requests for folder doesn't have '/' at end, so we add it.
            if (request_filename.back() != '/') {
                request_filename += '/';
            }

            delete_entity_folder(request_filename);
            server_logger->log_trace("-- crud delete folder valid");
            write_response("Delete successful for folder: " + path_extension,http_reply);
            return;
        } else {
            // not a directory in api folder, so return not found
            server_logger->log_trace("-- crud delete file not found");
            write_not_found_json_response(http_reply);
            return;
        }
    }
    else{
        // Case where no id was found: error for DELETE request
        server_logger->log_trace("-- crud delete file id not found");
        write_not_found_json_response(http_reply);
        return;
    }

    // the check_if_exists function will already set the correct response if doesn't exist, so just return out of function
    if (check_if_exists(sub_directory, file_id,http_reply) == false) {
        return;        
    }

    // Try deleting the entity ID file -- DELETE ENTITY ID handling
    std::string entity_id;
    std::string file_name;
    std::string file_path;

    entity_id = std::to_string(file_id); 
    file_name = entity_id + ".json";
    file_path = sub_directory + file_name;
    std::cout << file_path << std::endl;
    delete_entity(file_path, sub_directory, file_id); 
    std::string content_body = "Delete successful to file with ID: " + std::to_string(file_id);
    server_logger->log_trace("-- crud delete id valid");
    write_response(content_body,http_reply);
}

void Request_Handler_Crud::handle_request(const request &http_request, reply *http_reply) noexcept {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    // Configure the filename path with the configured root path
    std::string filename = use_configured_root(http_reply);

    std::cout<<"Filename path" <<filename <<std::endl;
    // Get method of request to call appropiate function
    std::string method = http_request.method_string().to_string();

    if (method == "POST")
    {
          // Handle POST request
        std::string json_data = (http_request.body());
        handle_post(filename,json_data,http_reply);
    }
    else if (method == "GET")
    {
        // Handle GET request
        handle_get(filename,http_reply);
    }
    else if (method == "PUT")
    {
        // Handle PUT request
        std::string json_data = (http_request.body());
        handle_put(filename, json_data,http_reply);
    }
    else if (method == "DELETE")
    {
        // Handle DELETE request
        handle_delete(filename,http_reply);
        
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
        server_logger->log_trace("-- crud ivalid api request");
        server_logger->log_info("[HandlerMetrics] Crud_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
    }
        
}
