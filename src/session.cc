#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include "session.h"
#include "http/request.hpp"
#include "http/reply.hpp"
#include "http/request_parser.hpp"
#include "logger.h"

using boost::asio::ip::tcp;
using http::server::request;
using http::server::reply;
using http::server::request_parser;

session::session(boost::asio::io_service& io_service,std::shared_ptr<const Request_Handler_Dispatcher> dispatcher)
    : socket_(io_service),dispatcher(dispatcher)
{
}

tcp::socket& session::socket() {
    return socket_;
}

void session::start()
{
    auto self(shared_from_this());
    boost::asio::async_read_until( socket_, 
        boost::asio::dynamic_buffer(data_),"\r\n\r\n",
        boost::bind(&session::handle_read, this,self,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(std::shared_ptr<session> self,const boost::system::error_code& error, size_t bytes_transferred)
{
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    if (!error)
    {
        // Parse the HTTP request
        // Obtain a pointer to the internal character array of the std::string object
        const char* data_begin = data_.data();
        // Pass the pointer as the begin iterator and data_ + bytes_transferred as the end iterator
        auto [result, unused_begin] = http_parser.parse(http_request, data_begin, data_begin + bytes_transferred);
        // Dispatch the request to the appropriate handler

        try {
            server_logger->log_request(http_request, socket_);
        } catch (const std::exception& e) {
            std::cout << "Error logging request: " << e.what() << std::endl;
        }

        if(result == true){
            std::shared_ptr<Request_Handler> handler = dispatcher->get_request_handler(http_request);
            if (handler == nullptr) {
                /*LOG NULL PTR*/
                http_reply = reply::stock_reply(reply::bad_request);
                server_logger->log_warning("Bad Request -- Request handler not found");
                server_logger->log_info("Response code: " + std::to_string(http_reply.status));
                return;
            }
            // Handle the request and generate a response
            try {
                handler->handle_request(http_request, &http_reply);
            } catch (const std::exception& e) {
                // std::cerr << "Error handling request: " << e.what() << std::endl;
                server_logger->log_error("Error handling request: " + std::string(e.what()));
                return;
            }

            // Log http response code
            server_logger->log_info("Response code: " + std::to_string(http_reply.status));
            if (http_reply.status == reply::ok) {
                server_logger->log_info("Good request");
            }

            boost::asio::async_write(
                socket_, http_reply.to_buffers(),
                boost::bind(&session::handle_write, shared_from_this(),boost::asio::placeholders::error));
            server_logger->log_debug("session::handle_read: async_write");
        }

        else if (result == false) {
            server_logger->log_info("Bad request");

            // Create a response indicating that the request was bad
            http_reply = reply::stock_reply(reply::bad_request);
            boost::asio::async_write(
                socket_, http_reply.to_buffers(),
                boost::bind(&session::handle_write, shared_from_this(),boost::asio::placeholders::error));
            server_logger->log_debug("session::handle_read: async_write");
        }

    }
    else {
        // std::cerr << "Error reading from socket: " << error.message() << std::endl;
        server_logger->log_error("Error reading from socket: " + std::string(error.message()));
        delete this;
    }
}
    


void session::handle_write(const boost::system::error_code& error)
{
    if (!error){
    boost::system::error_code shutdown_error;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, shutdown_error);
    }

    else{
      delete this;
    }
}