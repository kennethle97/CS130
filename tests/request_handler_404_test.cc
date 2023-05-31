#include "request_handler/request_handler_404.h"
#include "gtest/gtest.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

class RequestHandler404Test : public ::testing::Test {
    protected:

};

TEST_F(RequestHandler404Test, _404Request) {
    boost::beast::http::request<boost::beast::http::string_body> test_request;
    test_request.method(boost::beast::http::verb::get);
    test_request.target("/echo");
    test_request.version(11); //beast format for 1.1
    test_request.set(boost::beast::http::field::content_type, "text/plain");

    boost::beast::http::response<boost::beast::http::string_body> test_reply;

    Request_Handler_404 request_404_handler = Request_Handler_404("/");
    request_404_handler.handle_request(test_request, &test_reply);
    EXPECT_EQ(boost::beast::http::status::not_found, test_reply.result());
    EXPECT_EQ("HTTP/1.1 404 Not Found\r\nContent-Length: 86\r\nContent-Type: text/html\r\n\r\n", 
                 boost::lexical_cast<std::string>(test_reply.base()));
    EXPECT_EQ(false, test_reply[boost::beast::http::field::content_type].empty());
    EXPECT_EQ("text/html", test_reply[boost::beast::http::field::content_type]);
}
