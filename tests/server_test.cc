#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <boost/asio.hpp>
#include <fstream>

#include "server.h"
#include "request_handler_dispatcher.h"
#include "config_parser.h"

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

// testing whether creating a new server works
TEST_F(ServerTest, StartTest) {
    boost::asio::io_service io_service;
    server* new_server = new server(io_service, (short)8080, config);
    EXPECT_TRUE(true);
    delete new_server;
}

// testing whether adding a session with the server and calling start_accept function works
TEST_F(ServerTest, AcceptTest) {
    // MockSession mock_session(io_service_);
    boost::asio::io_service io_service_;
    server* new_server = new server(io_service_, (short)8080, config);
    testserver new_test_server;
    new_test_server.friend_start_accept(new_server);
    EXPECT_TRUE(true);
    delete new_server;
}

TEST_F(ServerTest, HandleTest) {
    // MockSession mock_session(io_service_);
    boost::asio::io_service io_service_;
    server* new_server = new server(io_service_, (short)8080, config);
    testserver new_test_server;
    auto new_session = std::make_shared<session>(io_service_, dispatcher);
    const boost::system::error_code error;
    new_test_server.friend_handle_accept(new_server, new_session, error);
    EXPECT_TRUE(true);
    delete new_server;
}