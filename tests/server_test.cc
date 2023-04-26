#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <boost/asio.hpp>

#include "server.h"

using boost::asio::ip::tcp;

// should be a gmock of session since server depends on it
// class MockSession : public session {
//     public:
//         MockSession(boost::asio::io_service io_service) : session(io_service) {};
//         MOCK_METHOD(void, handle_read, (const boost::system::error_code& error,
//       size_t bytes_transferred), (override));
//         MOCK_METHOD(void, handle_write, (const boost::system::error_code& error));
// }


// NOTE: if inputting a port number as a parameter for server, be sure to list a number that linux does not restrict
// I initially tried with port 80, but it was denied and so the test failed

class ServerTest : public ::testing::Test {
    protected:
};

// testing whether creating a new server works
TEST_F(ServerTest, StartTest) {
    boost::asio::io_service io_service;
    server* new_server = new server(io_service, (short)8080);
    EXPECT_TRUE(true);
    delete new_server;
}

// testing whether adding a session with the server and calling start_accept function works
TEST_F(ServerTest, AcceptTest) {
    // MockSession mock_session(io_service_);
    boost::asio::io_service io_service_;
    server* new_server = new server(io_service_, (short)8080);
    testserver new_test_server;
    new_test_server.friend_start_accept(new_server);
    EXPECT_TRUE(true);
    delete new_server;
}

TEST_F(ServerTest, HandleTest) {
    // MockSession mock_session(io_service_);
    boost::asio::io_service io_service_;
    server* new_server = new server(io_service_, (short)8080);
    testserver new_test_server;
    session* new_session = new session(io_service_);
    const boost::system::error_code error;
    new_test_server.friend_handle_accept(new_server, new_session, error);
    EXPECT_TRUE(true);
    delete new_server;
    delete new_session;
}