#include "request_handler_dispatcher.h"
#include "request_handler/request_handler_echo.h"
#include "request_handler/request_handler_static.h"
#include "request_handler/request_handler_health.h"
#include "request_handler/request_handler_404.h"
#include "../include/request_handler/request_handler_sleep.h"
#include "request_handler/echo_handler_factory.h"
#include "request_handler/static_handler_factory.h"
#include "request_handler/health_handler_factory.h"
#include "request_handler/request_404_handler_factory.h"
#include "request_handler/sleep_handler_factory.h"
#include "request_handler/crud_handler_factory.h"
#include "request_handler/sleep_handler_factory.h"
#include "request_handler/meme_handler_factory.h"


#include "gtest/gtest.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
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
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/echo");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");
    // Get the request handler object from the dispatcher
    std::shared_ptr<Request_Handler_Factory> handler = dispatcher->get_request_handler_factory(test_request);


    EXPECT_TRUE(handler != nullptr);
    // // Cast the base class pointer to a pointer of the derived class
    std::shared_ptr<Echo_Handler_Factory> echo_handler_factory = std::dynamic_pointer_cast<Echo_Handler_Factory>(handler);

    // // Check that the cast was successful
    EXPECT_TRUE(echo_handler_factory != nullptr);
}

TEST_F(RequestHandlerDispatcherTest, GetHandler404) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/non_existent_path/non_existent_file.txt");
    test_request.version(11); //beast format for 1.1
    
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    std::shared_ptr<Request_Handler_Factory> handler = dispatcher->get_request_handler_factory(test_request);

    EXPECT_TRUE(handler != nullptr);

    std::shared_ptr<Request_404_Handler_Factory> unknown_handler_factory = std::dynamic_pointer_cast<Request_404_Handler_Factory>(handler);

    EXPECT_TRUE(unknown_handler_factory != nullptr);
}
TEST_F(RequestHandlerDispatcherTest, GetHandlerHealth) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/health");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");
    // Get the request handler object from the dispatcher
    std::shared_ptr<Request_Handler_Factory> handler = dispatcher->get_request_handler_factory(test_request);


    EXPECT_TRUE(handler != nullptr);
    // // Cast the base class pointer to a pointer of the derived class
    std::shared_ptr<Health_Handler_Factory> health_handler_factory = std::dynamic_pointer_cast<Health_Handler_Factory>(handler);

    // // Check that the cast was successful
    EXPECT_TRUE(health_handler_factory != nullptr);
}
TEST_F(RequestHandlerDispatcherTest, GetHandlerCrud) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::post);
    test_request.target("/api");
    test_request.version(11); //beast format for 1.1
    
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    std::shared_ptr<Request_Handler_Factory> handler = dispatcher->get_request_handler_factory(test_request);

    EXPECT_TRUE(handler != nullptr);

    std::shared_ptr<Crud_Handler_Factory> unknown_handler_factory = std::dynamic_pointer_cast<Crud_Handler_Factory>(handler);

    EXPECT_TRUE(unknown_handler_factory != nullptr);
}

TEST_F(RequestHandlerDispatcherTest, GetHandlerStatic) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/static1/random.txt");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");
    // Get the request handler object from the dispatcher
    std::shared_ptr<Request_Handler_Factory> handler = dispatcher->get_request_handler_factory(test_request);

    EXPECT_TRUE(handler != nullptr);

    // Cast the base class pointer to a pointer of the derived class
    std::shared_ptr<Static_Handler_Factory> static_handler_factory = std::dynamic_pointer_cast<Static_Handler_Factory>(handler);

    // Check that the cast was successful
    EXPECT_TRUE(static_handler_factory != nullptr);

    Request_Handler_Static* static_handler = static_handler_factory->create("/static1", "/static1/random.txt");
    // Now you can access the root and prefix members of the derived class
    EXPECT_EQ(static_handler->get_prefix(), "/static1");
    EXPECT_EQ(static_handler->get_root(), "../public/folder1");
}


TEST_F(RequestHandlerDispatcherTest, GetHandlerMeme) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::post);
    test_request.target("/meme");
    test_request.version(11); //beast format for 1.1
    
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    std::shared_ptr<Request_Handler_Factory> handler = dispatcher->get_request_handler_factory(test_request);

    EXPECT_TRUE(handler != nullptr);

    std::shared_ptr<Meme_Handler_Factory> unknown_handler_factory = std::dynamic_pointer_cast<Meme_Handler_Factory>(handler);

    EXPECT_TRUE(unknown_handler_factory != nullptr);
}

