#include "request_handler/meme_handler_factory.h"
#include "request_handler_dispatcher.h"
#include "request_handler/request_handler_meme.h"
#include "config_parser.h"
#include "gtest/gtest.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <fstream>

class MemeHandlerFactoryTest : public ::testing::Test {
    public:
    NginxConfigParser parser;
    NginxConfig config;
    std::shared_ptr<Request_Handler_Dispatcher> dispatcher;
    
    // Set up the test fixture
    void SetUp() override {
        std::string config_file_path = "./dispatcher_config"; // replace with your config file path
        std::ifstream config_file(config_file_path);
        parser.Parse(&config_file, &config);
        dispatcher = std::make_shared<Request_Handler_Dispatcher>(config);
    }

};


TEST_F(MemeHandlerFactoryTest, MemeFactoryTest) {

    Meme_Handler_Factory handler_factory = Meme_Handler_Factory(config);

    boost::beast::http::request<boost::beast::http::string_body> test_request;

    test_request.method(boost::beast::http::verb::post);
    test_request.target("/meme/create");
    test_request.version(11); //beast format for 1.1
    test_request.body() = R"({
        "name": "crying_bro",
        "textbox": [
            {
                "text": "nobody:",
                "position": {
                    "top": 10,
                    "left": 20
                }
            },
            {
                "text": "me:",
                "position": {
                    "top": 50,
                    "left": 20
                }
            }
        ],
        "image": {
            "mime": "image/jpeg",
            "data": "R0lGODdhAQABAPAAAP8AAAAAACwAAAAAAQABAAACAkQBADs="
        }
    })";


    test_request.set(boost::beast::http::field::content_type, "application/json");


    boost::beast::http::response<boost::beast::http::string_body> test_reply;

    Request_Handler_Meme* request_meme_handler = handler_factory.create("/meme", "/meme/create");
    std::string data_path = request_meme_handler->get_data_path();
    
    request_meme_handler->handle_request(test_request, &test_reply);

    boost::filesystem::path tracker_path(data_path + "/meme_id_tracker.txt");
    std::ifstream file_new(tracker_path.string());
    int field_id;
    file_new >> field_id;
    file_new.close();

    EXPECT_EQ(boost::beast::http::status::ok, test_reply.result());
    EXPECT_EQ(test_reply.body(), "{file name: 1.json}\n");
    EXPECT_EQ(false, test_reply[boost::beast::http::field::content_type].empty());
    EXPECT_EQ("text/plain", test_reply[boost::beast::http::field::content_type]);
    EXPECT_EQ(handler_factory.meme_map->size(), 1);
    EXPECT_EQ(handler_factory.meme_locks->size(),1);
    EXPECT_EQ(field_id,1);

    boost::filesystem::remove (tracker_path);
    boost::filesystem::remove(data_path + "/1.json");

    std::ofstream file(tracker_path.string());
    file << 0;
    file.close();

}