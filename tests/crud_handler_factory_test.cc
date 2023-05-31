#include "gtest/gtest.h"
#include "request_handler/crud_handler_factory.h"
#include "request_handler_dispatcher.h"
#include "config_parser.h"
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <iostream>

class RequestCrudHandlerFactoryTest : public ::testing::Test {
    public:
    NginxConfigParser parser;
    NginxConfig config;
    std::shared_ptr<Request_Handler_Dispatcher> dispatcher;
    
    // Set up the test fixture
    void SetUp() override {
        std::string config_file_path = "./dispatcher_config"; // replace with your config file path
        std::ifstream config_file(config_file_path);
        parser.Parse(&config_file, &config);
        dispatcher = std::make_shared<Request_Handler_Dispatcher>(config);
    }

};

TEST_F(RequestCrudHandlerFactoryTest, RequestCrudFactoryTest) {

    Crud_Handler_Factory handler_factory = Crud_Handler_Factory(config);

    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::post);
    test_request.target("/api/test/1");
    test_request.version(11); //beast format for 1.1
    test_request.body() = "{\"key\": \"value\", \"test\": \"value\"}";  // Set the body with valid JSON content
    test_request.prepare_payload();  // Prepare payload
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    boost::beast::http::response<boost::beast::http::string_body> test_reply;
    
    path_uri url(test_request.target().to_string());
    path_uri location = "/api";
    Request_Handler_Crud* test_crud_handler = handler_factory.create(location, url);

    test_crud_handler->handle_request(test_request, &test_reply);
    EXPECT_EQ(boost::beast::http::status::ok, test_reply.result());
    std::string body(test_reply.body().data(), test_reply.body().size());    
    EXPECT_EQ(body, "{id: 1}\n");
    boost::filesystem::remove("../public/crud/test/1.json");
    boost::filesystem::remove("../public/crud/test");

}