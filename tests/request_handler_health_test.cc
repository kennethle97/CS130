#include "request_handler/request_handler_health.h"
#include "gtest/gtest.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

class RequestHandlerHealthTest : public ::testing::Test {
    protected:

};

TEST_F(RequestHandlerHealthTest, HealthRequest) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/health");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    boost::beast::http::response<boost::beast::http::string_body> test_reply;

    Request_Handler_Health request_health_handler = Request_Handler_Health("/health");
    request_health_handler.handle_request(test_request, &test_reply);
    EXPECT_EQ(boost::beast::http::status::ok, test_reply.result());
    EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 2\r\nContent-Type: text/plain\r\n\r\n", 
                 boost::lexical_cast<std::string>(test_reply.base()));
    EXPECT_EQ(false, test_reply[boost::beast::http::field::content_type].empty());
    EXPECT_EQ("text/plain", test_reply[boost::beast::http::field::content_type]);
}
