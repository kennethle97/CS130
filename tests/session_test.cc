#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <boost/asio.hpp>
#include "session.h"

using boost::asio::ip::tcp;

class SessionTest : public ::testing::Test {
    protected:
};

TEST_F(SessionTest, make_session) {
    boost::asio::io_service io_service_;
    session* s = new session(io_service_);
    EXPECT_TRUE(true);

    delete s;
}

TEST_F(SessionTest, return_socket) {
    boost::asio::io_service io_service_;
    session* s = new session(io_service_);
    s->socket();
    EXPECT_TRUE(true);

    delete s;
}

TEST_F(SessionTest, start_session) {
    boost::asio::io_service io_service_;
    session* s = new session(io_service_);
    s->start();
    EXPECT_TRUE(true);
    delete s;
}

TEST_F(SessionTest, handle_read_session) {
    boost::asio::io_service io_service_;
    testsession friend_session;
    session* s = new session(io_service_);
    boost::system::error_code error;
    friend_session.friend_handle_read(s, error, 0);
    EXPECT_TRUE(true);

    delete s;
}

TEST_F(SessionTest, handle_write_session) {
    boost::asio::io_service io_service_;
    testsession friend_session;
    session* s = new session(io_service_);
    boost::system::error_code error;
    friend_session.friend_handle_write(s, error);
    EXPECT_TRUE(true);

    delete s;
}