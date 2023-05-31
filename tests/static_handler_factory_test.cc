#include "request_handler/static_handler_factory.h"
#include "request_handler_dispatcher.h"
#include "request_handler/request_handler_static.h"
#include "config_parser.h"
#include "gtest/gtest.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <vector>
#include <string>
#include <iostream>

class StaticHandlerFactoryTest : public ::testing::Test {
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

TEST_F(StaticHandlerFactoryTest, IncorrectURLStaticFactoryTest) {
    Static_Handler_Factory handler_factory = Static_Handler_Factory(config);

    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/bad_path/random_file.txt");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    boost::beast::http::response<boost::beast::http::string_body> test_reply;

    path_uri url(test_request.target().to_string());
    
    Request_Handler_Static* request_static_handler = handler_factory.create("/static1", url);
    request_static_handler->handle_request(test_request, &test_reply);
    EXPECT_EQ(boost::beast::http::status::not_found, test_reply.result());
    EXPECT_EQ("<html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>\n", 
                                test_reply.body());
    EXPECT_EQ(false, test_reply[boost::beast::http::field::content_type].empty());
    EXPECT_EQ("text/html", test_reply[boost::beast::http::field::content_type]);
}
