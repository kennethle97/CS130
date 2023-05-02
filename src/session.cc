#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
<<<<<<< Updated upstream
#include "session.h"
#include "http/reply.h"

=======
#include <boost/beast/version.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include "../include/session.h"
>>>>>>> Stashed changes

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service,std::shared_ptr<const Request_Handler_Dispatcher> dispatcher)
    : socket_(io_service),dispatcher(dispatcher)
{
}

tcp::socket& session::socket()
{
    return socket_;
}

void session::start()
{
    auto self(shared_from_this());
<<<<<<< Updated upstream
    boost::asio::async_read_until( socket_, 
        boost::asio::dynamic_buffer(data_),"\r\n\r\n",
        boost::bind(&session::handle_read, this,self,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
=======
   boost::asio::async_read_until( socket_, 
        boost::asio::dynamic_buffer(data_),
        "\r\n\r\n",
        boost::bind(&session::handle_read, this,self,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
>>>>>>> Stashed changes
}

void session::handle_read(std::shared_ptr<session> self,const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        // Parse the HTTP request
       //boost::beast::http::request<boost::beast::http::dynamic_body> request;
        http_parser<true> request_parser;
        boost::beast::error_code parse_error;
        request_parser.put(boost::asio::buffer(data_, bytes_transferred), parse_error);
        request_parser.on_finish_impl(parse_error);

        if (parse_error && parse_error != boost::beast::http::error::need_more) {
        std::cerr << "Error parsing HTTP request: " << parse_error.message() << std::endl;
        return;
        }      



        auto& request = request_parser.get();
        auto& result = request.result();
        // Dispatch the request to the appropriate handler
        if(result == http::status::ok){
            std::shared_ptr<const Request_Handler> handler = dispatcher->get_request_handler(request);
            if (handler == nullptr) {
            std::cerr << "No request handler found for URI: " << request.target() << std::endl;
            return;
            }
             // Handle the request and generate a response
            http::response<http::string_body> response;
            try {
                handler->handle_request(request, &response);
            } catch (const std::exception& e) {
                std::cerr << "Error handling request: " << e.what() << std::endl;
                return;
            }
            boost::beast::http::async_write(
            socket_, response,
            boost::beast::bind_front_handler(&session::handle_write, shared_from_this()));
        }

<<<<<<< Updated upstream
        else if (result == bad_request){
=======
        else if (result == boost::beast::http::status::bad_request){
>>>>>>> Stashed changes
            // Create a response indicating that the request was bad
            boost::beast::http::response<boost::beast::http::string_body> response{boost::beast::http::status::bad_request, request.version()};
            response.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            response.set(boost::beast::http::field::content_type, "text/html");
            response.keep_alive(request.keep_alive());
            response.body() = "<html><body><h1>400 Bad Request</h1></body></html>";
            response.prepare_payload();
            boost::beast::http::async_write(
            socket_, response,
            boost::beast::bind_front_handler(&session::handle_write, shared_from_this()));
            }

        }
        else {
        std::cerr << "Error reading from socket: " << error.message() << std::endl;
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