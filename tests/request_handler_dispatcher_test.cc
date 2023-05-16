#include "request_handler_dispatcher.h"
#include "request_handler/request_handler_echo.h"
#include "request_handler/request_handler_static.h"
#include "gtest/gtest.h"
#include "http/reply.hpp"
#include "http/header.hpp"
#include "http/request.hpp"
#include "config_parser.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>



class RequestHandlerDispatcherTest : public ::testing::Test {
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

TEST_F(RequestHandlerDispatcherTest, GetHandlerEcho) {
    // NginxConfigParser parser;
    // NginxConfig config;

    // bool status = parser.Parse("config_test", &config);
    // std::cout << "status: " << status << std::endl;
    http::server::request test_request;
    test_request.method = "GET";
    test_request.uri = "/echo";
    test_request.http_version_major=1;
    test_request.http_version_minor=1;
    test_request.headers.resize(2);
    test_request.headers[0].name="Content-Type";
    test_request.headers[0].value="text/plain";
    // Get the request handler object from the dispatcher
    std::shared_ptr<Request_Handler> handler = dispatcher->get_request_handler(test_request);

    EXPECT_TRUE(handler != nullptr);
    // // Cast the base class pointer to a pointer of the derived class
    std::shared_ptr<Request_Handler_Echo> echo_handler = std::dynamic_pointer_cast<Request_Handler_Echo>(handler);

    // // Check that the cast was successful
    EXPECT_TRUE(echo_handler != nullptr);
}

TEST_F(RequestHandlerDispatcherTest, GetHandlerStaticFalse) {
    http::server::request test_request;
    test_request.method = "GET";
    test_request.uri = "/static123/random_file.txt";
    test_request.http_version_major=1;
    test_request.http_version_minor=1;
    test_request.headers.resize(2);
    test_request.headers[0].name="Content-Type";
    test_request.headers[0].value="text/plain";

    EXPECT_EQ(dispatcher->get_request_handler(test_request), nullptr);
}

TEST_F(RequestHandlerDispatcherTest, GetHandlerStaticTrue) {
    http::server::request test_request;
    test_request.method = "GET";
    test_request.uri = "/static1/random.txt";
    test_request.http_version_major=1;
    test_request.http_version_minor=1;
    test_request.headers.resize(2);
    test_request.headers[0].name="Content-Type";
    test_request.headers[0].value="text/plain";
    // Get the request handler object from the dispatcher
    std::shared_ptr<Request_Handler> handler = dispatcher->get_request_handler(test_request);

    EXPECT_TRUE(handler != nullptr);

    // Cast the base class pointer to a pointer of the derived class
    std::shared_ptr<Request_Handler_Static> static_handler = std::dynamic_pointer_cast<Request_Handler_Static>(handler);

    // Check that the cast was successful
    EXPECT_TRUE(static_handler != nullptr);

    // Now you can access the root and prefix members of the derived class
    EXPECT_EQ(static_handler->get_prefix(), "/static1");
    EXPECT_EQ(static_handler->get_root(), "../../public/folder1///");
}
