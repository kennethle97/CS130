#include "../../include/request_handler/request_handler_meme_list.h"
#include "logger.h"

using json = nlohmann::json;


Request_Handler_Meme_List::Request_Handler_Meme_List(const path_uri& data_path_, const path_uri& location_, const path_uri& url_) {
    this->data_path_ = data_path_;
    this->location_ = location_;
    this->url_ = url_;
    this->meme_map = std::unordered_map<std::string, meme_data>();
    this->ordered_likes = std::vector<std::pair<int,std::string>>();
    this->ordered_time = std::vector<std::pair<int,std::string>>();

    init_meme_map();
    num_memes = meme_map.size();


}


void Request_Handler_Meme_List::init_meme_map(){
    
    const boost::filesystem::path& root_path(this->data_path_);
    
    for(auto& entry : boost::filesystem::directory_iterator(root_path)){
        auto& path = entry.path();
        if(boost::filesystem::is_directory(path)){
            //Directory paths shouldn't exist inside data_path for the list of memes
            //but if for some reason it does we just skip.
            continue;
        }
        else{
            //Get the file_path and file id to parse the json and get the id for the file.
            auto& file_path = path.string();
            auto& file_name = path.filename().string();
            std::string id = file_name.substr(0,file_name.find_last_of("."));
            int file_id = std::stoi(id);

            std::string json_contents = get_entity(file_path);

            json parsed_data = json::parse(json_contents);

            std::string meme_name = parsed_data["name"];
            std::vector<std::string> likes = parsed_data["likes"];

            //Number of ip addresses in the .json will be the number of likes that meme entity currently has.
            int num_likes = likes.size();
            //Im assuming the second entry will be the path to the .json file  of the string
            ordered_likes.push_back(std::make_pair(num_likes,file_path));
            ordered_time.push_back(std::make_pair(file_id,file_path));
            
            //create a struct to add these values into the map.
            meme_data meme_info;
            meme_info.id = file_id;
            meme_info.num_likes = num_likes;
            meme_info.file_path = file_path;

            meme_map[meme_name] = meme_info;
            
            // file_locks[meme_name] = std::mutex();
        }
    }
}



std::string Request_Handler_Meme_List::use_configured_root(reply *http_reply){
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
        server_logger->log_info("[HandlerMetrics] Meme_List_Handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
        return NULL;
        }

    std::string filename = uri.substr(uri.find_last_of('/' + 1));
    filename = convert_name_from_url(filename);
    //Replace the ending in case the filename has spaces in it in the form of %20.
    uri = uri.substr(0,uri.find_last_of('/'+1))+filename;    
    return uri;
    }


void Request_Handler_Meme_List::sort_meme_likes(){
    std::sort(ordered_likes.begin(), ordered_likes.end(),
              [](const auto& a, const auto& b) {
                  return a.first > b.first;
              });
}

void Request_Handler_Meme_List::sort_meme_time(){
    std::sort(ordered_time.begin(), ordered_time.end(),
              [](const auto& a, const auto& b) {
                  return a.first < b.first;
              });
}


std::string Request_Handler_Meme_List::get_entity(std::string path) {
    // retrieve the contents of the file specified by path
    boost::filesystem::path file_path(path);

    std::ifstream file(file_path);
    std::stringstream content;
    content << file.rdbuf();
    file.close();
    
    return content.str();
}

void Request_Handler_Meme_List::update_entity(const std::string& path,const json& json_contents){
    boost::filesystem::path file_path(path);

    std::ofstream out_file(file_path);
    out_file << json_contents.dump();
    
}
bool Request_Handler_Meme_List::check_if_exists(std::string meme_name,reply *http_reply){
    auto it = meme_map.find(meme_name);
    if(it == meme_map.end()){
        write_not_found_meme_response(http_reply);
        return false;
    }
    return true;
}

Request_Handler_Meme_List::meme_data Request_Handler_Meme_List::get_meme_data(std::string meme_name){
    auto it = meme_map.find(meme_name);
    meme_data data = it->second;
    return data;
}

std::string Request_Handler_Meme_List::convert_name_from_url(std::string meme_name){
    /*Primary goal of this function is to be able to read character spaces in file names and return it*/
    
    const char* meme_name_char = meme_name.c_str();

    int meme_name_length = strlen(meme_name_char);    
    char* meme_name_decoded = (char*)malloc(meme_name_length+1);
    int j = 0;

    /*Parse through the filename to look for %20 symbol representing a space and returning the string with a space*/
    for(int i = 0; meme_name_char[i] != '\0';i++){
        if(meme_name_char[i] == '%'){
            if(meme_name_char[i+1] == '2' && meme_name_char [i+2] == '0'){
                meme_name_decoded[j] = ' ';
                i+=2;
            }
            // else{
            // meme_name_decoded[j]=(char)strtol(&meme_name_char[i+1],NULL, 16);
            // i+=2;
            //}
        }
        else{
            meme_name_decoded[j] = meme_name_char[i];
        }
        j++;
    }
    return std::string(meme_name_decoded);

}
bool Request_Handler_Meme_List::is_valid_json(std::string json_data){
    
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
    
bool Request_Handler_Meme_List::is_valid_ip_address(const std::string ip_address) {
    std::regex ip_address_regex(
        R"(^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$)"
    );

    return std::regex_match(ip_address, ip_address_regex);
}

void Request_Handler_Meme_List::write_base_http(reply *http_reply){
    http_reply->body().clear();
    http_reply->keep_alive(false);
    http_reply->result(boost::beast::http::status::ok);
}


void Request_Handler_Meme_List::write_response(json json_content,reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
	http_reply->set(boost::beast::http::field::content_type, "application/json");
	http_reply->set(boost::beast::http::field::content_disposition, "inline");
	http_reply->body() = json_content.dump() + "\n";
    http_reply->content_length(http_reply->body().size());
    server_logger->log_info("[HandlerMetrics] Meme_List_Handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));    
}
void Request_Handler_Meme_List::write_not_found_meme_response(reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    http_reply->result(boost::beast::http::status::not_found);
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    http_reply->set(boost::beast::http::field::content_disposition, "inline");
    http_reply->body() = "Could not resolve name of meme.\n";
    http_reply->content_length(http_reply->body().size());
    server_logger->log_info("[HandlerMetrics] Meme_List_Handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
}

void Request_Handler_Meme_List::write_bad_request(reply *http_reply){
        ServerLogger *server_logger = ServerLogger::get_server_logger();
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
        server_logger->log_trace("-- Meme List invalid api request");
        server_logger->log_info("[HandlerMetrics] Meme_List_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
    
}
void Request_Handler_Meme_List::handle_get(std::string meme_path, reply *http_reply){

    write_base_http(http_reply);

    meme_data data;
    json response_data;
    std::string json_contents = "";    
    
    std::string uri = meme_path;
    
    size_t last_slash_pos = uri.find_last_of('/');

    // Find the position of the second-to-last occurrence of '/'
    size_t second_last_slash_pos = uri.find_last_of('/', last_slash_pos - 1);

    // Extract the substring between the second-to-last and last slashes
    std::string second_to_last_segment = uri.substr(second_last_slash_pos + 1, last_slash_pos - second_last_slash_pos - 1);

    std::string meme_name = meme_path.substr(meme_path.find_last_of('/' + 1));
    
    //Checking the second to last segment to account for the case in which someone names their meme "list" so meme/list/list is still valid.
    
    if(meme_name == "list" && second_to_last_segment != "list"){
        //By default we can order by list or popularity. For now we will just order by time.
        sort_meme_time();
        std::string current_meme;
        for(int i = 0; i < num_memes;i++){
            //Get the name of the meme and look in the map for its filepath
            json current_meme_json;
            current_meme = ordered_time[i].second;
            auto it = meme_map.find(current_meme);
            data = it->second;            
            json_contents = get_entity(data.file_path);
            current_meme_json = json::parse(json_contents);
            current_meme_json["likes"] = data.num_likes;
            response_data.push_back(current_meme_json);                                                     
        }        
    }
    else if(meme_name == "popularity"){
        //This assumes that the path is meme/list/popularity
        sort_meme_likes();
        std::string current_meme;
        for(int i = 0; i < num_memes;i++){
            //Get the name of the meme and look in the map for its filepath
            json current_meme_json;
            current_meme = ordered_likes[i].second;
            auto it = meme_map.find(current_meme);
            data = it->second;
            json_contents = get_entity(data.file_path);
            current_meme_json = json::parse(json_contents);
            current_meme_json["likes"] = data.num_likes;
            response_data.push_back(current_meme_json);                                            
            
        }

    }
    else if(meme_name == "time"){
        //This assumes that the path is meme/list/time
        sort_meme_time();
        std::string current_meme;
        for(int i = 0; i < num_memes;i++){
            //Get the name of the meme and look in the map for its filepath
            json current_meme_json;
            current_meme = ordered_time[i].second;
            auto it = meme_map.find(current_meme);
            data = it->second;
            json_contents = get_entity(data.file_path);
            current_meme_json = json::parse(json_contents);
            current_meme_json["likes"] = data.num_likes;
            response_data.push_back(current_meme_json);                                                     
        }        
    }
    else{
        //This assumes that the request url is a name instead so the path will look like meme/list/name_of_meme
        if(check_if_exists(meme_name,http_reply)){
            data = get_meme_data(meme_name);
            json_contents = get_entity(data.file_path);
            response_data = json::parse(json_contents);
            response_data["likes"] = data.num_likes;
        } 
    }
     write_response(response_data,http_reply);
}



void Request_Handler_Meme_List::handle_post(std::string meme_path, std::string ip_address,reply* http_reply){

    write_base_http(http_reply);

    std::string uri = meme_path;
    json json_contents;
    meme_data data;
    
    size_t last_slash_pos = uri.find_last_of('/');

    // Find the position of the second-to-last occurrence of '/'
    size_t second_last_slash_pos = uri.find_last_of('/', last_slash_pos - 1);

    // Extract the substring between the second-to-last and last slashes which should be the meme name meme/list/name_of_meme/like
    std::string meme_name = uri.substr(second_last_slash_pos + 1, last_slash_pos - second_last_slash_pos - 1);

    //We have to decode the second to last segment if it includes spaces (%20) within the url
    
    meme_name = convert_name_from_url(meme_name);

    std::string like_extension = meme_path.substr(meme_path.find_last_of('/' + 1));

    if(like_extension == "like"){

        if(check_if_exists(meme_name,http_reply)){
            auto it = meme_map.find(meme_name);
            data = it->second;
            json_contents = get_entity(data.file_path);
            json& likes = json_contents["likes"];
            
            bool ip_exists = false;
            for (const auto& value: likes){
                if(value == ip_address){
                    ip_exists == true;
                    break;                    
                }
            }
            if(!ip_exists){
                likes.push_back(ip_address);
                update_entity(data.file_path,json_contents);            
                //Right now just sending the whole.json object back to the client so it would prob refresh the page? 
                //Could change to just update the number of likes but will need to figure out front end api.
                json_contents["likes"] = json_contents["likes"].size();
                write_response(json_contents,http_reply);
            } 
        }
    }
    else{
       write_bad_request(http_reply);
    }
}
void Request_Handler_Meme_List::handle_request(const request &http_request, reply *http_reply) noexcept {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    // Configure the filename path with the configured root path
    std::string filename = use_configured_root(http_reply);

    std::cout<<"Filename path" <<filename <<std::endl;
    // Get method of request to call appropiate function
    std::string method = http_request.method_string().to_string();
    std::string json_contents = http_request.body();

    if (method == "GET")
    {
        // Handle GET request for either list of memes sorted by either time 
        //or popularity or the individual meme itself.
        handle_get(filename,http_reply);
    }
   
    else if(method == "POST"){
        //Handle POST request i.e. whenever an invidividual viewing a meme through
        // meme/list/popularity or meme/list/time or meme/list/name_of_meme they we
        // add their ip to list of ips to update server side number of likes
        std::string json_contents = http_request.body();

        //We don't want people to do a POST command with just junk instead of an actual ip address
        //to update the number of likes.
        if(is_valid_ip_address(json_contents)){
            handle_post(filename,json_contents,http_reply);
        }
    }
    else{
       write_bad_request(http_reply);
    }
}