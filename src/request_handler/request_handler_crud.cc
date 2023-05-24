
#include "../../include/request_handler/request_handler_crud.h"
#include "../../include/http/mime_types.hpp"
#include "logger.h"

Request_Handler_Crud::Request_Handler_Crud(const path_uri& data_path_, const path_uri& location_, const path_uri& url_) {
    this->data_path_ = data_path_;
    this->location_ = location_;
    this->url_ = url_;
}

void Request_Handler_Crud::handle_request(const request &http_request, reply *http_reply) noexcept {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    boost::beast::string_view method = http_request.method_string();
    if (method == "POST") {
        create(http_request, http_reply);
    } else if (method == "GET") {
        read(http_request, http_reply);
    } else if (method == "PUT") {
        update(http_request, http_reply);
    } else if (method == "DELETE") {
        del(http_request, http_reply);
    } else {
        // TODO: Handle invalid request method
        server_logger->log_error("Invalid request method");
        return;
    }
    return;
}

void Request_Handler_Crud::create(const request &http_request, reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("Request_Handler_Crud::create()");

    // Step 1: Grab correct MIME type
    const boost::beast::string_view mime_type = http_request[boost::beast::http::field::content_type];
    // if (mime_type != "application/json") {
    //     http_reply->result(boost::beast::http::status::bad_request);
    //     server_logger->log_error("Invalid MIME type. Expected application/json.");
    //     return;
    // }

    // Step 3: Get next available id
    server_logger->log_info("URL for CRUD create request: " + url_);

    std::vector<std::string> parsed_url = parse_url();
    std::string crud_api = parsed_url[0];
    std::string entity_name = parsed_url[1];

    server_logger->log_info("Entity name: " + entity_name);
    server_logger->log_info("CRUD API: " + crud_api);
    int next_id = find_next_id(entity_name);
    server_logger->log_info("Next id: " + std::to_string(next_id));

    // Step 3: Write request body's JSON data to created file
    std::string file_path = get_file_path(entity_name, std::to_string(next_id));

    // Create folder for entity if it doesn't exist
    boost::filesystem::path dir(data_path_ + "/" + entity_name);
    if (boost::filesystem::create_directory(dir)) {
        server_logger->log_info("Created directory for entity: " + entity_name);
    }

    std::ofstream file(file_path);
    if (!file) {
        http_reply->result(boost::beast::http::status::internal_server_error);
        server_logger->log_error("Failed to open file for entity: " + entity_name + " with id: " + std::to_string(next_id) + " at path: " + file_path);
        return;
    }

    file << http_request.body();
    file.close();
    server_logger->log_info("Successfully wrote JSON data for request at " + http_request.target().to_string() + " to file: " + file_path);

    // Step 4: Return response of newly created entity (ex. {"id": 1}"})
    nlohmann::json reply_json;
    reply_json["id"] = next_id;

    http_reply->result(boost::beast::http::status::created);
    http_reply->set(boost::beast::http::field::content_type, "application/json");
    http_reply->body() = reply_json.dump();
    server_logger->log_info("Successfully responded to request at " + http_request.target().to_string());
    return;
}

void Request_Handler_Crud::read(const request &http_request, reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("Request_Handler_Crud::read()");

    std::vector<std::string> parsed_url = parse_url();
    std::string entity_name = parsed_url[1];
    std::string entity_id = parsed_url[2];
    path_uri file_path = get_file_path(entity_name, entity_id);

    if (!boost::filesystem::exists(file_path)) {
        http_reply->result(boost::beast::http::status::not_found);
        const char not_found[] =
                        "<html>"
                        "<head><title>Not Found</title></head>"
                        "<body><h1>404 Not Found</h1></body>"
                        "</html>\n";
        http_reply->body() = not_found;
        //http_reply->content_length(http_reply->body().size());
        http_reply->set(boost::beast::http::field::content_type, "text/html");
        //*******stock reply************
        // server_logger->log_error("File path does not exist for entity: " + entity_name + " with id: " + std::to_string(next_id));
        return;
    }
}

void Request_Handler_Crud::update(const request &http_request, reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("Request_Handler_Crud::update()");
}

void Request_Handler_Crud::del(const request &http_request, reply *http_reply) {
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    server_logger->log_debug("Request_Handler_Crud::del()");
}

// Helper function for POST to find next open id for a given entity
int Request_Handler_Crud::find_next_id(std::string entity) {
    int next_id = 1;
    while (true) {
        // Check if the requested file exists and is a regular file
        std::string file_path = get_file_path(entity, std::to_string(next_id));
        boost::filesystem::path path(file_path);
        
        if (boost::filesystem::exists(path)) {
            next_id++;
        } else {
            break;
        }
    }

    return next_id;
}

path_uri Request_Handler_Crud::get_file_path(std::string entity, std::string id) {
    return data_path_ + "/" + entity + "/" + id;
}

std::vector<std::string> Request_Handler_Crud::parse_url() {
    std::vector<std::string> result;
    // parse the url to get the entity name
    int paren_count = 0;
    std::string entity_name = "";
    std::string crud_api = "";
    std::string entity_id = "";
    for (int i = 0; i < url_.size(); i++) {
        if (url_[i] == '/') {
             paren_count++;
             continue;
        }
        if (paren_count == 3) {
            entity_id += url_[i];
        } else if (paren_count == 2) {
            entity_name += url_[i];
        } else if (paren_count == 1) {
            crud_api += url_[i];
        }
    }
    result.push_back(crud_api);
    result.push_back(entity_name);
    result.push_back(entity_id);
    return result;
}