#include "gtest/gtest.h"
#include "request_handler/request_handler_crud.h"
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

using boost::asio::ip::tcp;
using request = boost::beast::http::request<boost::beast::http::string_body>;
using reply = boost::beast::http::response<boost::beast::http::string_body>;
using request_parser = boost::beast::http::request_parser<boost::beast::http::string_body>;

class CRUDRequestHandlerTestFixture : public ::testing::Test {
    protected:
        const NginxConfig in_config;
        void SetUp() override {
            // testing::internal::CaptureStdout();
            boost::filesystem::create_directory(data_path);
            std::string units_dir = data_path + "/units";
            bool res = boost::filesystem::create_directory(units_dir);
            
            // Test file to be used for GET, UPDATE, DELETE
            std::string file_path = units_dir + "/1";
            nlohmann::json json_body = {
            {"tester", "wesley"},
            {"team_name", "Team_404"}
            };

            std::ofstream file(file_path);
            if (file.is_open()) {
                file << json_body.dump(4);
                file.close();
            } else {
                throw std::runtime_error("Failed to create test file: " + file_path);
            }

        }

        void TearDown() override { 
          delete crud_handler_;

          boost::filesystem::remove_all(data_path);
        }
        path_uri data_path = "../Testing/crud";    // Root path to serve files from
        path_uri location = "";  // Location to match with root directory
        path_uri url = "/api/units/1";   // Full path from http request

        Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, url);

};

TEST_F(CRUDRequestHandlerTestFixture, ValidGetResponse) {
    request request;
    reply reply;

    // Set the HTTP method, target, and version
    request.method(boost::beast::http::verb::get);
    request.target(url);
    request.version(11); // HTTP/1.1

    crud_handler_->handle_request(request, &reply);

    EXPECT_EQ(reply.base().at(boost::beast::http::field::content_type).to_string(), "application/json");
    std::string expectedResponseBody =  "{\"team_name\":\"Team_404\",\"tester\":\"wesley\"}";
    EXPECT_EQ(reply.body(), expectedResponseBody);
}

TEST_F(CRUDRequestHandlerTestFixture, InvalidGetResponse) {
    request request1;
    reply reply1;
    path_uri fake_url = "/api/wrong/20";
    request1.method(boost::beast::http::verb::get);
    request1.target(fake_url);
    request1.version(11); // HTTP/1.1
    Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, fake_url);

    crud_handler_->handle_request(request1, &reply1);
    const char not_found[] =
                "<html>"
                "<head><title>Not Found</title></head>"
                "<body><h1>404 Not Found</h1></body>"
                "</html>\n";
    delete crud_handler_;

    EXPECT_EQ(reply1.result(), boost::beast::http::status::not_found);
    EXPECT_EQ(reply1.body(), not_found);
}

TEST_F(CRUDRequestHandlerTestFixture, ValidPutRequestResponse) {
    nlohmann::json json_body = {
            {"tester", "kenny"},
            {"team_name", "Team_404"}
            };

    std::string string_body = json_body.dump(4);

    request update_request_;
    reply update_reply_;

    request request;
    reply reply;

    request.method(boost::beast::http::verb::get);
    request.target(url);
    request.version(11); // HTTP/1.1

    crud_handler_->handle_request(request, &reply);
    update_request_.method(boost::beast::http::verb::put);
    update_request_.target(url);
    update_request_.version(11);
    update_request_.set(boost::beast::http::field::host, "localhost");
    update_request_.set(boost::beast::http::field::content_type, "application/json");
    update_request_.set(boost::beast::http::field::content_length, std::to_string(string_body.size()));
    update_request_.body() = string_body;

    crud_handler_->handle_request(update_request_, &update_reply_);
    EXPECT_EQ(update_reply_.result(), boost::beast::http::status::ok);
    request.method(boost::beast::http::verb::get);
    request.target(url);
    request.version(11); // HTTP/1.1
    crud_handler_->handle_request(request, &reply);

    EXPECT_EQ(reply.base().at(boost::beast::http::field::content_type).to_string(), "application/json");
    std::string expectedResponseBody =  "{\"team_name\":\"Team_404\",\"tester\":\"kenny\"}";
    EXPECT_EQ(reply.body(), expectedResponseBody);

}

TEST_F(CRUDRequestHandlerTestFixture, InvalidPutRequestResponse) {    
    nlohmann::json json_body = {
        {"tester", "kenny"},
        {"team_name", "Team_404"}
        };
    path_uri fake_url = "/api/wrong/20";
    Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, fake_url);

    std::string string_body = json_body.dump(4);

    request update_request_;
    reply update_reply_;

    update_request_.method(boost::beast::http::verb::put);
    update_request_.target(fake_url);
    update_request_.version(11);
    update_request_.set(boost::beast::http::field::host, "localhost");
    update_request_.set(boost::beast::http::field::content_type, "application/json");
    update_request_.set(boost::beast::http::field::content_length, std::to_string(string_body.size()));
    update_request_.body() = string_body;

    crud_handler_->handle_request(update_request_, &update_reply_);
    EXPECT_EQ(update_reply_.result(), boost::beast::http::status::bad_request);
    delete crud_handler_;
}

TEST_F(CRUDRequestHandlerTestFixture, ValidPostRequestResponse) {

    nlohmann::json json_body = {
            {"tester", "kenny"},
            {"team_name", "Team_404"}
            };

    std::string string_body = json_body.dump(4);
    request update_request_;
    reply update_reply_;
    path_uri new_url = "/api/units";
    Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, new_url);

    update_request_.method(boost::beast::http::verb::post);
    update_request_.target("/api/units");
    update_request_.version(11);
    update_request_.set(boost::beast::http::field::host, "localhost");
    update_request_.set(boost::beast::http::field::content_type, "application/json");
    update_request_.set(boost::beast::http::field::content_length, std::to_string(string_body.size()));
    update_request_.body() = string_body;

    crud_handler_->handle_request(update_request_, &update_reply_);

    EXPECT_EQ(update_reply_.base().at(boost::beast::http::field::content_type).to_string(), "application/json");
    nlohmann::json responseBody = nlohmann::json::parse(update_reply_.body());
    EXPECT_TRUE(responseBody.contains("id"));
    EXPECT_TRUE(responseBody["id"].is_number_integer());
    delete crud_handler_;

}
TEST_F(CRUDRequestHandlerTestFixture, InvalidPostRequestPath) {

        nlohmann::json json_body = {
            {"tester", "kenny"},
            {"team_name", "Team_404"}
            };

    std::string string_body = json_body.dump(4);
    request update_request_;
    reply update_reply_;
    path_uri new_url = "/api";
    Request_Handler_Crud* crud_handler_ = new Request_Handler_Crud(data_path, location, new_url);

    update_request_.method(boost::beast::http::verb::post);
    update_request_.target("/api");
    update_request_.version(11);
    update_request_.set(boost::beast::http::field::host, "localhost");
    update_request_.set(boost::beast::http::field::content_type, "application/json");
    update_request_.set(boost::beast::http::field::content_length, std::to_string(string_body.size()));
    update_request_.body() = string_body;

    crud_handler_->handle_request(update_request_, &update_reply_);

    EXPECT_EQ(update_reply_.result(), boost::beast::http::status::bad_request);
    delete crud_handler_;
}
