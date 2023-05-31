#include "gtest/gtest.h"
#include "request_handler/request_handler_crud.h"
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <iostream>

using boost::asio::ip::tcp;
using request = boost::beast::http::request<boost::beast::http::string_body>;
using reply = boost::beast::http::response<boost::beast::http::string_body>;
using request_parser = boost::beast::http::request_parser<boost::beast::http::string_body>;

class RequestHandlerCrudTest : public ::testing::Test {
    protected:

        void SetUp() override {
            // testing::internal::CaptureStdout();
            data_path = "../Testing/crud";    // Root path to serve files from
            location = "/api";  // Location to match with root directory
            boost::filesystem::create_directory(data_path);
        }

        void TearDown() override { 

          boost::filesystem::remove_all(data_path);
        }

        path_uri data_path;
        path_uri location;

};

TEST_F(RequestHandlerCrudTest, CreateCrudHandler)
{
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    path_uri url = "/api/units";
    EXPECT_NO_THROW(Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map));
}
TEST_F(RequestHandlerCrudTest, RunHandlePostCreate)
{
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/2");
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload
    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][2];
    EXPECT_EQ(file,"2.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
    boost::filesystem::remove("../Testing/crud/units/2.json");
    delete crud_handler;
}
TEST_F(RequestHandlerCrudTest, RunHandlePostConflict)
{ 
    
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/1");
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload
    reply response_;
    
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);

    crud_handler->handle_request(request_, &response_);

    EXPECT_EQ(response_.result(), boost::beast::http::status::conflict);

    delete crud_handler;
}
TEST_F(RequestHandlerCrudTest, RunHandlePostInvalidJson)
{ 
    
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/2");
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{ definitely not a json payload }";  // Set the body with invalid JSON content
    request_.prepare_payload();  // Prepare payload
    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);

    EXPECT_EQ(response_.result(), boost::beast::http::status::unsupported_media_type);
    delete crud_handler;
}
TEST_F(RequestHandlerCrudTest, RunHandlePostNewEntity)
{ 

    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units///////////////////////"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload
    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
}

TEST_F(RequestHandlerCrudTest, RunHandlePutSuccess)
{
    
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/1");
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
       

    request request_2;
    request_2.method(boost::beast::http::verb::put);
    request_2.version(11);  // HTTP/1.1
    request_2.target("/api/units/1");
    request_2.set(boost::beast::http::field::host, "localhost:8080");
    request_2.body() = "{\"key\": \"changed_value\", \"changed_test\": \"value\"}";  // Set the body with valid JSON content
    request_2.prepare_payload();  // Prepare payload


    reply response_2;
    path_uri url_2(request_.target().to_string());    
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url_2,map);
    crud_handler_2->handle_request(request_2, &response_2);
    auto file_2 = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file_2,"1.json");
    EXPECT_EQ(response_2.result(), boost::beast::http::status::ok);
    std::string body(response_2.body().data(), response_2.body().size());    
    EXPECT_EQ(body, "Update successful to file with ID: 1\n");

    std::ifstream file_content("../Testing/crud/units/1.json");
    std::stringstream buffer;
    buffer << file_content.rdbuf();
    std::string changed_body = buffer.str();

    EXPECT_EQ(changed_body, "{\"key\": \"changed_value\", \"changed_test\": \"value\"}");    
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;    
    
}

TEST_F(RequestHandlerCrudTest, RunHandlePutIDFailure)
{
    
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/1");
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];
    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
       
    request request_2;
    request_2.method(boost::beast::http::verb::put);
    request_2.version(11);  // HTTP/1.1
    request_2.target("/api/units/10");
    request_2.set(boost::beast::http::field::host, "localhost:8080");
    request_2.body() = "{\"key\": \"changed_value\", \"changed_test\": \"value\"}";  // Set the body with valid JSON content
    request_2.prepare_payload();  // Prepare payload

    reply response_2;
    path_uri url_2(request_2.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url_2,map);
    crud_handler_2->handle_request(request_2, &response_2);
    auto file_2 = (*map)["../Testing/crud/units/"][1];
    
    EXPECT_EQ(file_2,"1.json");
    EXPECT_EQ(response_2.result(), boost::beast::http::status::not_found);
    std::string body(response_2.body().data(), response_2.body().size());
    EXPECT_EQ(body, "Could not resolve ID -- ID not found.\n");

    std::ifstream file_content("../Testing/crud/units/1.json");
    std::stringstream buffer;
    buffer << file_content.rdbuf();
    std::string changed_body = buffer.str();
    EXPECT_EQ(changed_body, "{\"key\": \"value\", \"test\": \"value\"}");  // contents of file should NOT have changed
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");    
    delete crud_handler;
    delete crud_handler_2;
}

TEST_F(RequestHandlerCrudTest, RunHandlePutSubDirectoryFailure)
{

    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/1");
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
       
    request request_2;
    request_2.method(boost::beast::http::verb::put);
    request_2.version(11);  // HTTP/1.1
    request_2.target("/api/this_sub_directory_does_not_exist/1");
    request_2.set(boost::beast::http::field::host, "localhost:8080");
    request_2.body() = "{\"key\": \"changed_value\", \"changed_test\": \"value\"}";  // Set the body with valid JSON content
    request_2.prepare_payload();  // Prepare payload

    
    reply response_2;

    path_uri url_2(request_2.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url_2,map);
    crud_handler_2->handle_request(request_2, &response_2);
    auto file_2 = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file_2,"1.json");
    EXPECT_EQ(response_2.result(), boost::beast::http::status::not_found);
    std::string body(response_2.body().data(), response_2.body().size());
    EXPECT_EQ(body, "Could not resolve ID -- ID not found.\n");

    std::ifstream file_content("../Testing/crud/units/1.json");
    std::stringstream buffer;
    buffer << file_content.rdbuf();
    std::string changed_body = buffer.str();
    EXPECT_EQ(changed_body, "{\"key\": \"value\", \"test\": \"value\"}");  // contents of file should NOT have changed
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;    

}

TEST_F(RequestHandlerCrudTest, RunHandleGetSuccess)
{
    
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/1");
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
       
    request request_2;
    request_2.method(boost::beast::http::verb::get);
    request_2.version(11);  // HTTP/1.1
    request_2.target("/api/units/1");
    request_2.set(boost::beast::http::field::host, "localhost:8080");
    request_2.prepare_payload();  // Prepare payload

    reply response_2;
    path_uri url_2(request_.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url_2,map);
    crud_handler_2->handle_request(request_2, &response_2);
    auto file_2 = (*map)["../Testing/crud/units/"][1];
    
    EXPECT_EQ(file_2,"1.json");
    EXPECT_EQ(response_2.result(), boost::beast::http::status::ok);
    std::string body(response_2.body().data(), response_2.body().size());
    EXPECT_EQ(body, "{\"key\": \"value\", \"test\": \"value\"}\n");
    
    std::ifstream file_content("../Testing/crud/units/1.json");
    std::stringstream buffer;
    buffer << file_content.rdbuf();
    std::string no_change_body = buffer.str();
    EXPECT_EQ(no_change_body, "{\"key\": \"value\", \"test\": \"value\"}");    
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;
}

// This test checks whether deleting an existing entity id is successfully deleted
TEST_F(RequestHandlerCrudTest, RunHandleDeleteExists)
{ 
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units///////////////////////"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);

    auto entity_map = (*map)["../Testing/crud/units/"];
    auto it = entity_map.find(1);

    EXPECT_TRUE(it != entity_map.end());

    request_.method(boost::beast::http::verb::delete_);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/1"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");

    url = (request_.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler_2->handle_request(request_, &response_);
    entity_map = (*map)["../Testing/crud/units/"];
    it = entity_map.find(1);

    EXPECT_TRUE(it == entity_map.end());
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;
}

// This test checks whether deleting a nonexistent entity id gives a 404 error
TEST_F(RequestHandlerCrudTest, RunHandleDeleteNotExists)
{ 
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units///////////////////////"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];

    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);

    auto entity_map = (*map)["../Testing/crud/units/"];
    auto it = entity_map.find(1);
    EXPECT_TRUE(it != entity_map.end());

    request_.method(boost::beast::http::verb::delete_);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/3"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");

    url = (request_.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler_2->handle_request(request_, &response_);
    entity_map = (*map)["../Testing/crud/units/"];
    it = entity_map.find(1);
    EXPECT_TRUE(it != entity_map.end());

    // EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::not_found);
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;
}

// This test checks whether deleting an existing entity directory is successfully deleted
TEST_F(RequestHandlerCrudTest, RunHandleDeleteDirExists)
{ 
    
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units///////////////////////"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];
    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);

    auto entity_map = (*map)["../Testing/crud/units/"];
    auto it = entity_map.find(1);
    EXPECT_TRUE(it != entity_map.end());

    request_.method(boost::beast::http::verb::delete_);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");

    url = (request_.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler_2->handle_request(request_, &response_);
    entity_map = (*map)["../Testing/crud/units/"];
    EXPECT_EQ(0, entity_map.size());

    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;
}

// This test checks whether deleting a nonexistent entity directory returns not found
TEST_F(RequestHandlerCrudTest, RunHandleDeleteDirNotExists)
{ 

    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units///////////////////////"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];
    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);

    auto entity_map = (*map)["../Testing/crud/units/"];
    auto it = entity_map.find(1);
    EXPECT_TRUE(it != entity_map.end());

    request_.method(boost::beast::http::verb::delete_);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/notexists/"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");

    url = (request_.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler_2->handle_request(request_, &response_);

    EXPECT_EQ(response_.result(), boost::beast::http::status::not_found);
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;

}

// This test checks whether getting a folder returns a list
TEST_F(RequestHandlerCrudTest, RunHandleGetListExists)
{ 
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units///////////////////////"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];
    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);

    auto entity_map = (*map)["../Testing/crud/units/"];
    auto it = entity_map.find(1);
    EXPECT_TRUE(it != entity_map.end());

    request_.method(boost::beast::http::verb::get);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units/"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");

    url = (request_.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler_2->handle_request(request_, &response_);
    entity_map = (*map)["../Testing/crud/units/"];
    EXPECT_EQ(1, entity_map.size());

    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;
}

// This test checks whether getting a nonexistent folder returns not found
TEST_F(RequestHandlerCrudTest, RunHandleGetListNotExists)
{ 
    auto map = std::make_shared<std::map<std::string, std::map<int, std::string>>>();
    
    request request_;
    request_.method(boost::beast::http::verb::post);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/units///////////////////////"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");
    request_.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    request_.prepare_payload();  // Prepare payload

    reply response_;
    path_uri url(request_.target().to_string());
    Request_Handler_Crud* crud_handler = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler->handle_request(request_, &response_);
    auto file = (*map)["../Testing/crud/units/"][1];
    EXPECT_EQ(file,"1.json");
    EXPECT_EQ(response_.result(), boost::beast::http::status::ok);

    auto entity_map = (*map)["../Testing/crud/units/"];
    auto it = entity_map.find(1);
    EXPECT_TRUE(it != entity_map.end());

    request_.method(boost::beast::http::verb::get);
    request_.version(11);  // HTTP/1.1
    request_.target("/api/notexists/"); //testing for backslashes as well
    request_.set(boost::beast::http::field::host, "localhost:8080");

    url = (request_.target().to_string());
    Request_Handler_Crud* crud_handler_2 = new Request_Handler_Crud(data_path,location,url,map);
    crud_handler_2->handle_request(request_, &response_);

    EXPECT_EQ(response_.result(), boost::beast::http::status::not_found);
    boost::filesystem::remove("../Testing/crud/units/1.json");
    boost::filesystem::remove("../Testing/crud/units");
    delete crud_handler;
    delete crud_handler_2;
}

// TEST_F(CRUDRequestHandlerTestFixture, ValidGetResponse) {
//     request request;
//     reply reply;

//     // Set the HTTP method, target, and version
//     request_.method(boost::beast::http::verb::get);
//     request_.target("/api/units/1");
//     request_.version(11); // HTTP/1.1

//     std::string url = "/api/units/1";

//      Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, url,map);

//     EXPECT_EQ(reply.base().at(boost::beast::http::field::content_type).to_string(), "application/json");
//     std::string expectedResponseBody =  "{\"team_name\":\"Team_404\",\"tester\":\"wesley\"}";
//     EXPECT_EQ(reply.body(), expectedResponseBody);
// }

// TEST_F(CRUDRequestHandlerTestFixture, InvalidGetResponse) {
//     request request1;
//     reply reply1;
//     path_uri fake_url = "/api/wrong/20";
//     request1.method(boost::beast::http::verb::get);
//     request1.target(fake_url);
//     request1.version(11); // HTTP/1.1
//     Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, fake_url);

//     crud_handler_->handle_request(request1, &reply1);
//     const char not_found[] =
//                 "<html>"
//                 "<head><title>Not Found</title></head>"
//                 "<body><h1>404 Not Found</h1></body>"
//                 "</html>\n";
//     delete crud_handler_;

//     EXPECT_EQ(reply1.result(), boost::beast::http::status::not_found);
//     EXPECT_EQ(reply1.body(), not_found);
// }

// TEST_F(CRUDRequestHandlerTestFixture, ValidPutRequestResponse) {
//     nlohmann::json json_body = {
//             {"tester", "kenny"},
//             {"team_name", "Team_404"}
//             };

//     std::string string_body = json_body.dump(4);

//     request update_request_;
//     reply update_reply_;

//     request request;
//     reply reply;

//     request_.method(boost::beast::http::verb::get);
//     request_.target(url);
//     request_.version(11); // HTTP/1.1

//     crud_handler_->handle_request(request, &reply);
//     update_request_.method(boost::beast::http::verb::put);
//     update_request_.target(url);
//     update_request_.version(11);
//     update_request_.set(boost::beast::http::field::host, "localhost");
//     update_request_.set(boost::beast::http::field::content_type, "application/json");
//     update_request_.set(boost::beast::http::field::content_length, std::to_string(string_body.size()));
//     update_request_.body() = string_body;

//     crud_handler_->handle_request(update_request_, &update_reply_);
//     EXPECT_EQ(update_reply_.result(), boost::beast::http::status::ok);
//     request_.method(boost::beast::http::verb::get);
//     request_.target(url);
//     request_.version(11); // HTTP/1.1
//     crud_handler_->handle_request(request, &reply);

//     EXPECT_EQ(reply.base().at(boost::beast::http::field::content_type).to_string(), "application/json");
//     std::string expectedResponseBody =  "{\"team_name\":\"Team_404\",\"tester\":\"kenny\"}";
//     EXPECT_EQ(reply.body(), expectedResponseBody);

// }

// TEST_F(CRUDRequestHandlerTestFixture, InvalidPutRequestResponse) {    
//     nlohmann::json json_body = {
//         {"tester", "kenny"},
//         {"team_name", "Team_404"}
//         };
//     path_uri fake_url = "/api/wrong/20";
//     Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, fake_url);

//     std::string string_body = json_body.dump(4);

//     request update_request_;
//     reply update_reply_;

//     update_request_.method(boost::beast::http::verb::put);
//     update_request_.target(fake_url);
//     update_request_.version(11);
//     update_request_.set(boost::beast::http::field::host, "localhost");
//     update_request_.set(boost::beast::http::field::content_type, "application/json");
//     update_request_.set(boost::beast::http::field::content_length, std::to_string(string_body.size()));
//     update_request_.body() = string_body;

//     crud_handler_->handle_request(update_request_, &update_reply_);
//     EXPECT_EQ(update_reply_.result(), boost::beast::http::status::bad_request);
//     delete crud_handler_;
// }

// TEST_F(CRUDRequestHandlerTestFixture, ValidPostRequestResponse) {

//     nlohmann::json json_body = {
//             {"tester", "kenny"},
//             {"team_name", "Team_404"}
//             };

//     std::string string_body = json_body.dump(4);
//     request update_request_;
//     reply update_reply_;
//     path_uri new_url = "/api/units";
//     Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, new_url);

//     update_request_.method(boost::beast::http::verb::post);
//     update_request_.target("/api/units");
//     update_request_.version(11);
//     update_request_.set(boost::beast::http::field::host, "localhost");
//     update_request_.set(boost::beast::http::field::content_type, "application/json");
//     update_request_.set(boost::beast::http::field::content_length, std::to_string(string_body.size()));
//     update_request_.body() = string_body;

//     crud_handler_->handle_request(update_request_, &update_reply_);

//     EXPECT_EQ(update_reply_.base().at(boost::beast::http::field::content_type).to_string(), "application/json");
//     nlohmann::json responseBody = nlohmann::json::parse(update_reply_.body());
//     EXPECT_TRUE(responseBody.contains("id"));
//     EXPECT_TRUE(responseBody["id"].is_number_integer());
//     delete crud_handler_;

// }
// TEST_F(CRUDRequestHandlerTestFixture, InvalidPostRequestPath) {

//         nlohmann::json json_body = {
//             {"tester", "kenny"},
//             {"team_name", "Team_404"}
//             };

//     std::string string_body = json_body.dump(4);
//     request update_request_;
//     reply update_reply_;
//     path_uri new_url = "/api";
//     Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, new_url);

//     update_request_.method(boost::beast::http::verb::post);
//     update_request_.target("/api");
//     update_request_.version(11);
//     update_request_.set(boost::beast::http::field::host, "localhost");
//     update_request_.set(boost::beast::http::field::content_type, "application/json");
//     update_request_.set(boost::beast::http::field::content_length, std::to_string(string_body.size()));
//     update_request_.body() = string_body;

//     crud_handler_->handle_request(update_request_, &update_reply_);

//     EXPECT_EQ(update_reply_.result(), boost::beast::http::status::bad_request);
//     delete crud_handler_;
// }
