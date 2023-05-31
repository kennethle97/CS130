#include "request_handler/request_handler_static.h"
#include "gtest/gtest.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

class RequestHandlerStaticTest : public ::testing::Test {
    protected:

};

TEST_F(RequestHandlerStaticTest, StaticNotFoundTest) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/static1/drandom_file.txt");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    boost::beast::http::response<boost::beast::http::string_body> test_reply;

    Request_Handler_Static request_static_handler("../public/folder1", "/static1", "/static1/drandom.txt");
    request_static_handler.handle_request(test_request, &test_reply);
    EXPECT_EQ(boost::beast::http::status::not_found, test_reply.result());
    EXPECT_EQ("<html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>\n", 
                                test_reply.body());
    EXPECT_EQ(false, test_reply[boost::beast::http::field::content_type].empty());
    EXPECT_EQ("text/html", test_reply[boost::beast::http::field::content_type]);
}

TEST_F(RequestHandlerStaticTest, StaticFoundTest) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/static1/random_file.txt");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    boost::beast::http::response<boost::beast::http::string_body> test_reply;

    Request_Handler_Static request_static_handler("../public/folder1", "/static1", "/static1/random.txt");
    request_static_handler.handle_request(test_request, &test_reply);
    EXPECT_EQ(boost::beast::http::status::ok, test_reply.result());
    EXPECT_EQ("Hi, this is a random .txt file!\n", test_reply.body());
    EXPECT_EQ(false, test_reply[boost::beast::http::field::content_type].empty());
    EXPECT_EQ("text/plain", test_reply[boost::beast::http::field::content_type]);
}

TEST_F(RequestHandlerStaticTest, StaticNestedFoundTest) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/static1/nested/nest.txt");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    boost::beast::http::response<boost::beast::http::string_body> test_reply;

    Request_Handler_Static request_static_handler("../public/folder1", "/static1", "/static1/nested/nest.txt");
    request_static_handler.handle_request(test_request, &test_reply);
    EXPECT_EQ(boost::beast::http::status::ok, test_reply.result());
    EXPECT_EQ("Hi, this is a nested .txt file!\n", test_reply.body());
    EXPECT_EQ(false, test_reply[boost::beast::http::field::content_type].empty());
    EXPECT_EQ("text/plain", test_reply[boost::beast::http::field::content_type]);
}
