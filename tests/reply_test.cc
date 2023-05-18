#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "gtest/gtest.h"
#include <boost/asio.hpp>
#include <vector>

class ReplyTest : public ::testing::Test {
    protected:

};
//dont need these tests cuz delete reply.cc

// test that echo reply is made correctly and returns the correct response
/*
TEST_F(ReplyTest, EchoReply) {
    boost::beast::http::response<boost::beast::http::string_body> test_reply;
    test_reply.result(boost::beast::http::status::ok);
    http_reply.set(boost::beast::http::field::content_type, "text/plain");


    test_reply.content="GET /echo HTTP/1.1\r\n\r\n";
    
    std::vector<boost::asio::const_buffer> buffers = test_reply.to_buffers();

    EXPECT_EQ(std::string(static_cast<const char*>(boost::asio::buffer("HTTP/1.1 200 OK\r\n").data())), std::string(static_cast<const char*>(buffers[0].data())));

    EXPECT_EQ(std::string(static_cast<const char*>(boost::asio::buffer("Content-Type").data())), std::string(static_cast<const char*>(buffers[1].data())));

    EXPECT_EQ(std::string(static_cast<const char*>(boost::asio::buffer(": \r\n").data())), std::string(static_cast<const char*>(buffers[2].data())));

    EXPECT_EQ(std::string(static_cast<const char*>(boost::asio::buffer("text/plain").data())), std::string(static_cast<const char*>(buffers[3].data())));

    EXPECT_EQ(std::string(static_cast<const char*>(boost::asio::buffer("\r\n").data())), std::string(static_cast<const char*>(buffers[4].data())));

    EXPECT_EQ(std::string(static_cast<const char*>(boost::asio::buffer("\r\n").data())), std::string(static_cast<const char*>(buffers[5].data())));

    EXPECT_EQ(std::string(static_cast<const char*>(boost::asio::buffer("GET /echo HTTP/1.1\r\n\r\n").data())), std::string(static_cast<const char*>(buffers[6].data())));
}*/
/*
TEST_F(ReplyTest, CheckAllStatusTypes) {
    boost::beast::http::response<boost::beast::http::string_body> test_reply;

    ttest_reply.result(boost::beast::http::status::ok);
    std::vector<boost::asio::const_buffer> test_header;
    test_header.push_back(boost::asio::buffer("HTTP/1.1 200 OK\r\n"));
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 201 Created\r\n"));
    test_reply.status=http::server::reply::status_type::created;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));
    
    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 202 Accepted\r\n"));
    test_reply.status=http::server::reply::status_type::accepted;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 204 No Content\r\n"));
    test_reply.status=http::server::reply::status_type::no_content;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 300 Multiple Choices\r\n"));
    test_reply.status=http::server::reply::status_type::multiple_choices;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 301 Moved Permanently\r\n"));
    test_reply.status=http::server::reply::status_type::moved_permanently;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 302 Moved Temporarily\r\n"));
    test_reply.status=http::server::reply::status_type::moved_temporarily;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 304 Not Modified\r\n"));
    test_reply.status=http::server::reply::status_type::not_modified;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 400 Bad Request\r\n"));
    test_reply.status=http::server::reply::status_type::bad_request;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 401 Unauthorized\r\n"));
    test_reply.status=http::server::reply::status_type::unauthorized;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 403 Forbidden\r\n"));
    test_reply.status=http::server::reply::status_type::forbidden;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 404 Not Found\r\n"));
    test_reply.status=http::server::reply::status_type::not_found;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 500 Internal Server Error\r\n"));
    test_reply.status=http::server::reply::status_type::internal_server_error;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 501 Not Implemented\r\n"));
    test_reply.status=http::server::reply::status_type::not_implemented;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 502 Bad Gateway\r\n"));
    test_reply.status=http::server::reply::status_type::bad_gateway;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));

    test_header.pop_back();
    test_header.push_back(boost::asio::buffer("HTTP/1.1 503 Service Unavailable\r\n"));
    test_reply.status=http::server::reply::status_type::service_unavailable;
    EXPECT_EQ(std::string(static_cast<const char*>(test_header[0].data())), std::string(static_cast<const char*>(test_reply.to_buffers()[0].data())));


}*/
/*
TEST_F(ReplyTest, StockReplyTest) {
    http::server::reply test_reply;
    test_reply.status=http::server::reply::status_type::ok;
    test_reply.headers.resize(2);
    test_reply.headers[0].name = "Content-Length";
    test_reply.headers[0].value = "0";
    test_reply.headers[1].name = "Content-Type";
    test_reply.headers[1].value = "text/html";
    test_reply.content="";
    
    http::server::reply stock_reply_val = test_reply.stock_reply(test_reply.status);
    
    EXPECT_EQ(test_reply.status, stock_reply_val.status);
    EXPECT_EQ(test_reply.headers[0].name, stock_reply_val.headers[0].name);
    EXPECT_EQ(test_reply.headers[0].value, stock_reply_val.headers[0].value);
    EXPECT_EQ(test_reply.headers[1].name, stock_reply_val.headers[1].name);
    EXPECT_EQ(test_reply.headers[1].value, stock_reply_val.headers[1].value);
    EXPECT_EQ(test_reply.content, stock_reply_val.content);
    
}
*/