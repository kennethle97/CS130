#include "request_handler/request_handler_echo.h"
#include "gtest/gtest.h"
#include "http/reply.hpp"
#include "http/header.hpp"
#include "http/request.hpp"
#include <vector>

class RequestHandlerEchoTest : public ::testing::Test {
    protected:

};

TEST_F(RequestHandlerEchoTest, EchoRequest) {
    http::server::request test_request;
    test_request.method = "GET";
    test_request.uri = "/echo";
    test_request.http_version_major=1;
    test_request.http_version_minor=1;
    test_request.headers.resize(2);
    test_request.headers[0].name="Content-Type";
    test_request.headers[0].value="text/plain";

    http::server::reply test_reply;

    Request_Handler_Echo request_echo_handler;
    request_echo_handler.handle_request(test_request, &test_reply);
    EXPECT_EQ(http::server::reply::status_type::ok, test_reply.status);
    EXPECT_EQ("GET /echo HTTP/1.1\r\nContent-Type: text/plain\r\n: \r\n\r\n", test_reply.content);
    EXPECT_EQ("Content-Length", test_reply.headers[0].name);
    EXPECT_EQ("52", test_reply.headers[0].value);
    EXPECT_EQ("Content-Type", test_reply.headers[1].name);
    EXPECT_EQ("text/plain", test_reply.headers[1].value);
}