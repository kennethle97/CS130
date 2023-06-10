#include "../../include/request_handler/request_handler_meme.h"
#include "logger.h"

using json = nlohmann::json;


Request_Handler_Meme::Request_Handler_Meme(const path_uri& data_path_, const path_uri& location_, const path_uri& url_,
                                                        std::shared_ptr<std::unordered_map<std::string,std::unique_lock<std::mutex>>> meme_locks,
                                                        std::shared_ptr<std::unordered_map<std::string,meme_data>> meme_map,
                                                        std::shared_ptr<std::shared_mutex> meme_map_lock) 
{
    this->data_path_ = data_path_;
    this->location_ = location_;
    this->url_ = url_;
    this->meme_map = meme_map;
    this->meme_locks = meme_locks;
    this->meme_map_lock = meme_map_lock;
    this->ordered_likes = std::vector<std::pair<int,std::string>>();
    this->ordered_time = std::vector<std::pair<int,std::string>>();

    this->meme_map_copy = (std::unordered_map<std::string,meme_data>(*meme_map));
    init_meme_map();
    num_memes = meme_map->size();


}


void Request_Handler_Meme::init_meme_map(){
    std::shared_lock<std::shared_mutex> lock(*meme_map_lock);
    if(meme_map->empty()){
        const boost::filesystem::path root_path(this->data_path_);
        
        for(auto entry : boost::filesystem::directory_iterator(root_path)){
            auto path = entry.path();
            std::cout<<"path : "<<path <<std::endl;
            if(boost::filesystem::is_directory(path)){
                //Directory paths shouldn't exist inside data_path for the list of memes
                //but if for some reason it does we just skip.
                continue;
            }
            else{
                //Get the file_path and file id to parse the json and get the id for the file.
                auto file_path = path.string();
                auto file_name = path.filename().string();

                if(file_name != "meme_id_tracker.txt"){
                        std::cout<<"failed  conditional"<<std::endl;
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

                        (*meme_map)[meme_name] = meme_info;
                        
                        (*meme_locks)[meme_name] = std::unique_lock<std::mutex>();
                    }
                }
            }
         }
    }



std::string Request_Handler_Meme::use_configured_root(reply *http_reply){
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
        server_logger->log_trace("-- Meme file location not found");
        server_logger->log_info("[HandlerMetrics] Meme_List_Handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
        return NULL;
        }

    std::string filename = uri.substr(uri.find_last_of('/') + 1);
    filename = convert_name_from_url(filename);
    //Replace the ending in case the filename has spaces in it in the form of %20.
    uri = uri.substr(0,uri.find_last_of('/')+1)+filename;    
    return uri;
    }


void Request_Handler_Meme::sort_meme_likes(){
        int num_likes;
        for(auto it = meme_map_copy.begin(); it!= meme_map_copy.end();++it){
            std::string meme_name = it->first;
            meme_data data = it->second;
            ordered_likes.push_back(std::make_pair(data.num_likes,meme_name));
        }    
    std::sort(ordered_likes.begin(), ordered_likes.end(),
              [](const auto& a, const auto& b) {
                  return a.first > b.first;
              });
}

void Request_Handler_Meme::sort_meme_time(){
    for(auto it = meme_map_copy.begin(); it!= meme_map_copy.end();++it){
        std::string meme_name = it->first;
        meme_data data = it->second;
        ordered_time.push_back(std::make_pair(data.id,meme_name));
    }    
    std::sort(ordered_time.begin(), ordered_time.end(),
              [](const auto& a, const auto& b) {
                  return a.first > b.first;
              });
}


std::string Request_Handler_Meme::get_entity(std::string path) {
    // retrieve the contents of the file specified by path
    boost::filesystem::path file_path(path);

    std::ifstream file(file_path);
    std::stringstream content;
    content << file.rdbuf();
    file.close();
    
    return content.str();
}

void Request_Handler_Meme::update_entity(const std::string& path,const json& json_contents){
    boost::filesystem::path file_path(path);

    std::ofstream out_file(file_path);
    out_file << json_contents.dump();
    
}
bool Request_Handler_Meme::check_if_exists(std::string meme_name,reply *http_reply){
    auto it = meme_map_copy.find(meme_name);
    if(it == meme_map_copy.end()){
        write_not_found_meme_response(http_reply);
        return false;
    }
    return true;
}

Request_Handler_Meme::meme_data Request_Handler_Meme::get_meme_data(std::string meme_name){
    auto it = meme_map_copy.find(meme_name);
    meme_data data = it->second;
    return data;
}

std::string Request_Handler_Meme::convert_name_from_url(std::string meme_name){
    /*Primary goal of this function is to be able to read character spaces in file names and return it*/
    
    const char* meme_name_char = meme_name.c_str();

    int meme_name_length = meme_name.length();    
    char* meme_name_decoded = (char*)malloc(meme_name_length);
    int j = 0;

    /*Parse through the filename to look for %20 symbol representing a space and returning the string with a space*/
    for(int i = 0; i < meme_name_length; i++){
        if(meme_name_char[i] == '%'){
            if(meme_name_char[i+1] == '2' && meme_name_char [i+2] == '0'){
                meme_name_decoded[j] = ' ';
                i+=2;
            }
            else{
            meme_name_decoded[j]=(char)strtol(&meme_name_char[i+1],NULL, 16);
            i+=2;
            }
        }
        else{
            meme_name_decoded[j] = meme_name_char[i];
        }
        j++;
    }
    size_t length = j;
    std::string meme_name_str(meme_name_decoded,length);
    return meme_name_str;
}
bool Request_Handler_Meme::is_valid_json(std::string json_data){
    
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
    
bool Request_Handler_Meme::is_valid_ip_address(const std::string ip_address) {
    std::regex ip_address_regex(
        R"(^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$)"
    );

    return std::regex_match(ip_address, ip_address_regex);
}

void Request_Handler_Meme::write_base_http(reply *http_reply){
    http_reply->body().clear();
    http_reply->keep_alive(false);
    http_reply->result(boost::beast::http::status::ok);
}


void Request_Handler_Meme::write_response(json json_content,reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
	http_reply->set(boost::beast::http::field::content_type, "application/json");
	http_reply->set(boost::beast::http::field::content_disposition, "inline");
	http_reply->body() = json_content.dump(4) + "\n";
    http_reply->content_length(http_reply->body().size());
    server_logger->log_info("[HandlerMetrics] Meme_List_Handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));    
}
void Request_Handler_Meme::write_not_found_meme_response(reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    http_reply->result(boost::beast::http::status::not_found);
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    http_reply->set(boost::beast::http::field::content_disposition, "inline");
    http_reply->body() = "Could not resolve name of meme.\n";
    http_reply->content_length(http_reply->body().size());
    server_logger->log_info("[HandlerMetrics] Meme_List_Handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));
}

void Request_Handler_Meme::write_bad_request(reply *http_reply){
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

void Request_Handler_Meme::write_invalid_json_response(reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    http_reply->result(boost::beast::http::status::unsupported_media_type);                   
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    http_reply->set(boost::beast::http::field::content_disposition, "inline");
    http_reply->body() =  "Unsupported Media Type: The provided file type is not supported.\n";
    http_reply->content_length(http_reply->body().size());
}


void Request_Handler_Meme::write_file_exists_response(std::string meme_name,reply *http_reply){
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    http_reply->result(boost::beast::http::status::conflict);                   
    http_reply->set(boost::beast::http::field::content_type, "text/plain");
    http_reply->set(boost::beast::http::field::content_disposition, "inline");
    http_reply->body() = std::string("File already exists at path with name: ")+ meme_name +std::string("}.\n");
    http_reply->content_length(http_reply->body().size());
    server_logger->log_info("[HandlerMetrics] Meme_Handler");
    server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));

}
std::string Request_Handler_Meme::update_field_id(std::string rootpath){
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
}

void Request_Handler_Meme::create_meme(std::string json_data,reply*http_reply){
    //create the path with the filename already passed in with the extension,.json
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    json json_contents = json::parse(json_data);
    std::string meme_name = json_contents["name"];

    if(meme_map->find(meme_name) == meme_map->end()){
        std::unique_lock<std::shared_mutex> lock(*meme_map_lock);
        meme_data new_data;
        int field_id = std::stoi(update_field_id(data_path_));

        std::string file_name = std::to_string(field_id) + ".json";
        std::string path = data_path_ + "/" + file_name;

        new_data.id = field_id;
        new_data.file_path = path;
        new_data.num_likes = 0;
        (*meme_map)[meme_name] = new_data;
        (*meme_locks)[meme_name] = std::unique_lock<std::mutex>{};

        json_contents["likes"] = json::array();

        boost::filesystem::path file_path(path);
        std::ofstream file(file_path);
        file << json_contents.dump(4);
        file.close();

        http_reply->set(boost::beast::http::field::content_type, "text/plain");
        http_reply->set(boost::beast::http::field::content_disposition, "inline");
        http_reply->body() = std::string("{file name: ") + file_name + "}\n";
        http_reply->content_length(http_reply->body().size());  
        server_logger->log_trace("--  meme create valid ");
        server_logger->log_info("[HandlerMetrics] Meme_create_handler");
        server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));  
    }
    else{
        write_file_exists_response(meme_name,http_reply);
        
    }
}

void Request_Handler_Meme::handle_get(std::string meme_path, reply *http_reply){

    write_base_http(http_reply);

    meme_data data;
    json response_data;
    std::string json_contents;    
    
    std::string uri = meme_path;
    
    size_t last_slash_pos = uri.find_last_of('/');

    // Find the position of the second-to-last occurrence of '/'
    size_t second_last_slash_pos = uri.find_last_of('/', last_slash_pos - 1);

    // Extract the substring between the second-to-last and last slashes
    std::string second_to_last_segment = uri.substr(second_last_slash_pos + 1, last_slash_pos - second_last_slash_pos - 1);

    std::string meme_name = meme_path.substr(meme_path.find_last_of('/') + 1);
    
    //Checking the second to last segment to account for the case in which someone names their meme "list" so meme/list/list is still valid.
    
    if(meme_name == "list" && second_to_last_segment != "list"){
        //By default we can order by list or popularity. For now we will just order by time.
        sort_meme_time();
        std::string current_meme;
        for(int i = 0; i < num_memes;i++){
            //Get the name of the meme and look in the map for its filepath
            json current_meme_json;
            current_meme = ordered_time[i].second;
            auto it = meme_map_copy.find(current_meme);
            data = it->second;            
            json_contents = get_entity(data.file_path);
            current_meme_json = json::parse(json_contents);
            // current_meme_json["likes"] = std::to_string(data.num_likes);
            response_data.push_back(current_meme_json);                                                     
        }
         write_response(response_data,http_reply);        
    }
    else if(meme_name == "popularity" && second_to_last_segment == "list"){
        //This assumes that the path is meme/list/popularity
        sort_meme_likes();
        std::string current_meme;
        for(int i = 0; i < num_memes;i++){
            //Get the name of the meme and look in the map for its filepath
            json current_meme_json;
            current_meme = ordered_likes[i].second;
            auto it = meme_map_copy.find(current_meme);
            data = it->second;
            json_contents = get_entity(data.file_path);
            current_meme_json = json::parse(json_contents);
            // current_meme_json["likes"] = std::to_string(data.num_likes);
            response_data.push_back(current_meme_json);                                            
        }
         write_response(response_data,http_reply);

    }
    else if(meme_name == "time" && second_to_last_segment == "list"){
        //This assumes that the path is meme/list/time
        sort_meme_time();
        std::string current_meme;
        for(int i = 0; i < num_memes;i++){
            //Get the name of the meme and look in the map for its filepath
            json current_meme_json;
            current_meme = ordered_time[i].second;
            auto it = meme_map_copy.find(current_meme);
            data = it->second;
            json_contents = get_entity(data.file_path);
            current_meme_json = json::parse(json_contents);
            // current_meme_json["likes"] = std::to_string(data.num_likes);
            response_data.push_back(current_meme_json);                                                     
        }
         write_response(response_data,http_reply);        
    }
    else{
        //This assumes that the request url is a name instead so the path will look like meme/list/name_of_meme
        if(check_if_exists(meme_name,http_reply)){
            data = get_meme_data(meme_name);
            json_contents = get_entity(data.file_path);
            response_data = json::parse(json_contents);
            // response_data["likes"] = std::to_string(data.num_likes);
            write_response(response_data,http_reply);
        } 
    }
}



void Request_Handler_Meme::handle_post(std::string meme_path, std::string ip_address,reply* http_reply){

    write_base_http(http_reply);

    std::string uri = meme_path;
    json json_contents;
    
    size_t last_slash_pos = uri.find_last_of('/');

    // Find the position of the second-to-last occurrence of '/'
    size_t second_last_slash_pos = uri.find_last_of('/', last_slash_pos - 1);

    // Extract the substring between the second-to-last and last slashes which should be the meme name meme/list/name_of_meme/like
    std::string meme_name = uri.substr(second_last_slash_pos + 1, last_slash_pos - second_last_slash_pos - 1);

    //We have to decode the second to last segment if it includes spaces (%20) within the url
    
    meme_name = convert_name_from_url(meme_name);

    std::string like_extension = meme_path.substr(meme_path.find_last_of('/') + 1);

    if(like_extension == "like"){

        if(check_if_exists(meme_name,http_reply)){
            //If it exists then we obtain the lock on it so that other handlers 
            //have to wait until the number of likes for this entry is updated.

            
            std::unique_lock<std::mutex>& lock = (*meme_locks)[meme_name];

            auto it = meme_map->find(meme_name);
            meme_data &data = it->second;
            json_contents = json::parse(get_entity(data.file_path));
            json& likes = json_contents["likes"];
            
            bool ip_exists = false;
            for (const auto& value: likes){
                if(value == ip_address){
                    ip_exists = true;
                    break;                    
                }
            }
            if(!ip_exists){
                likes.push_back(ip_address);
                update_entity(data.file_path,json_contents);
                data.num_likes += 1;           
                //Right now just sending the whole.json object back to the client so it would prob refresh the page? 
                //Could change to just update the number of likes but will need to figure out front end api.
                // json_contents["likes"] = std::to_string(data.num_likes);
                write_response(json_contents,http_reply);
            } 
        }
    }
    else{
       write_bad_request(http_reply);
    }
}
void Request_Handler_Meme::handle_request(const request &http_request, reply *http_reply) noexcept {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    // Configure the filename path with the configured root path
    std::string filename = use_configured_root(http_reply);

    std::cout<<"Filename path" <<filename <<std::endl;
    // Get method of request to call appropiate function
    std::string method = http_request.method_string().to_string();
    std::string json_contents = http_request.body();

    if (method == "GET")
    {
        std::string meme_list = "meme/list";

        if(filename.find(meme_list) != std::string::npos){
            // Handle GET request for either list of memes sorted by either time 
            //or popularity or the individual meme itself.
            handle_get(filename,http_reply);
        }
        else{
            write_bad_request(http_reply);
        }
    }
   
    else if(method == "POST"){
        //Handle POST request i.e. whenever an invidividual viewing a meme through
        // meme/list/popularity or meme/list/time or meme/list/name_of_meme they we
        // add their ip to list of ips to update server side number of likes

        
        std::string meme_create = "meme/create";
        std::string meme_like = "meme/list/";
        std::cout <<meme_create<<std::endl;
        std::cout<<meme_like<<std::endl;
        std::string json_contents = http_request.body();
       

        if(filename.find(meme_like) != std::string::npos){
            //We don't want people to do a POST command with just junk instead of an actual ip address
            //to update the number of likes.
            if(is_valid_ip_address(json_contents)){
                handle_post(filename,json_contents,http_reply);
            }
            else{
                http_reply->set(boost::beast::http::field::content_type, "text/plain");
                http_reply->set(boost::beast::http::field::content_disposition, "inline");
                http_reply->body() = std::string("{Invalid payload, Must be Ip_address : ") + json_contents + "}\n";
                http_reply->content_length(http_reply->body().size());  
                server_logger->log_trace("--  meme handler invalid like payload" );
                server_logger->log_info("[HandlerMetrics] Meme_handler");
                server_logger->log_info("[ResponseMetrics] " + std::to_string(http_reply->result_int()));


            }
        }
        //Makes sure that meme/create substring is at the end of the url only so you can't make a post command with meme/create/some_random_thing
       
        else if ((filename.length() >= meme_create.length()) && (filename.substr(filename.length() - meme_create.length()) == meme_create)){

                bool is_json = is_valid_json(json_contents);

                if(!is_json){
                //LOG invalid file type. Not a json file type.
                    server_logger->log_trace("-- Meme Create invalid file json type");
                    write_invalid_json_response(http_reply);
                    return;
                }
                // size_t meme_pos = filename.find(meme_create);
                // filename.replace(meme_pos,meme_create.length(),"meme");


                //example file name: 2.json

                create_meme(json_contents,http_reply);

            }
        else{
            write_bad_request(http_reply);
        }
    }
    else{
       write_bad_request(http_reply);
    }
}