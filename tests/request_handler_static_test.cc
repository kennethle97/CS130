#include "request_handler/request_handler_static.h"
#include "gtest/gtest.h"
#include "http/reply.hpp"
#include "http/header.hpp"
#include "http/request.hpp"

class RequestHandlerStaticTest : public ::testing::Test {
    protected:

};

TEST_F(RequestHandlerStaticTest, StaticNotFoundTest) {
    http::server::request test_request;
    test_request.method = "GET";
    test_request.uri = "/static123/random_file.txt";
    test_request.http_version_major=1;
    test_request.http_version_minor=1;
    test_request.headers.resize(2);
    test_request.headers[0].name="Content-Type";
    test_request.headers[0].value="text/plain";

    http::server::reply test_reply;

    Request_Handler_Static request_static_handler("/", "/static/");
    request_static_handler.handle_request(test_request, &test_reply);
    EXPECT_EQ(http::server::reply::status_type::not_found, test_reply.status);
    EXPECT_EQ("<html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>", test_reply.content);
    EXPECT_EQ("Content-Length", test_reply.headers[0].name);
    EXPECT_EQ("85", test_reply.headers[0].value);
    EXPECT_EQ("Content-Type", test_reply.headers[1].name);
    EXPECT_EQ("text/html", test_reply.headers[1].value);
}