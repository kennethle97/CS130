#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <boost/asio.hpp>
#include <fstream>
#include "session.h"
#include "server.h"
#include "request_handler_dispatcher.h"
#include "config_parser.h"

using boost::asio::ip::tcp;

class SessionTest : public ::testing::Test {
    protected:
    NginxConfigParser parser;
    NginxConfig config;
    std::shared_ptr<Request_Handler_Dispatcher> dispatcher;
    
    // Set up the test fixture
    void SetUp() override {
        std::string config_file_path = "./config_test"; // replace with your config file path
        std::ifstream config_file(config_file_path);
        parser.Parse(&config_file, &config);
        dispatcher = std::make_shared<Request_Handler_Dispatcher>(config);
    }
};

TEST_F(SessionTest, make_session) {
    boost::asio::io_service io_service_;
    session* s = new session(io_service_,dispatcher);
    EXPECT_TRUE(true);

    delete s;
}

TEST_F(SessionTest, return_socket) {
    boost::asio::io_service io_service_;
    session* s = new session(io_service_,dispatcher);
    s->socket();
    EXPECT_TRUE(true);

    delete s;
}

TEST_F(SessionTest, start_session) {
    boost::asio::io_service io_service_;
    auto s = std::make_shared<session>(io_service_, dispatcher);
    s->start();
    EXPECT_TRUE(true);
}

// TEST_F(SessionTest, handle_read_session) {
//     boost::asio::io_service io_service_;
//     testsession friend_session;
//     auto s = std::make_shared<session>(io_service_, dispatcher);
//     boost::system::error_code error;
//     friend_session.friend_handle_read(s.get(), s, error, 0);
//     EXPECT_TRUE(true);
// }


TEST_F(SessionTest, handle_write_session) {
    boost::asio::io_service io_service_;
    testsession friend_session;
    session* s = new session(io_service_, dispatcher);
    boost::system::error_code error;
    friend_session.friend_handle_write(s,error);
    EXPECT_TRUE(true);
    delete s;
}