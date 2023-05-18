#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include "session.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "logger.h"
#include <string>

using boost::asio::ip::tcp;
using request = boost::beast::http::request<boost::beast::http::string_body>;
using reply = boost::beast::http::response<boost::beast::http::string_body>;
using request_parser = boost::beast::http::request_parser<boost::beast::http::string_body>;

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
        // Obtain a pointer to the internal character array of the std::string object of the buffer containing the request
        const char* data_begin = data_.data();
        // Pass the pointer as the begin iterator and data_ + bytes_transferred as the end iterator
        
        boost::beast::error_code parse_error;
        http_parser.put(boost::asio::buffer(data_begin, bytes_transferred), parse_error);  //parser parse buffer

        if(http_parser.is_done())
        {   
            http_request = http_parser.get();
        }
        
        /*result returns a tribool where we ignore the right side of the tuple and only care about the 
        result boolean value returning true if its a valid http request and false if it is a bad request*/

        /* Dispatch the request to the appropriate handler */

        try {
            server_logger->log_request(http_request, socket_);
        } catch (const std::exception& e) {}
        /*If the request object is a valid request i.e. result == true then call the dispatcher to get the appropiate request_handler*/
        if(!parse_error){

            std::shared_ptr<Request_Handler_Factory> handler_factory = dispatcher->get_request_handler_factory(http_request);
            if (handler_factory == nullptr) {
                // If handler_factory is a nullptr, then a handler factory was not dispatched (most
                // likely due to a bad config)
                http_reply.result(boost::beast::http::status::internal_server_error);
                const char internal_server_error[] =
                            "<html>"
                            "<head><title>Internal Server Error</title></head>"
                            "<body><h1>500 Internal Server Error</h1></body>"
                            "</html>\n";
                http_reply.body() = internal_server_error;
                http_reply.content_length(http_reply.body().size());
                http_reply.set(boost::beast::http::field::content_type, "text/html");

                server_logger->log_error("Internal Server Error -- a handler factory was not dispatched");
                server_logger->log_info("Response code: " + std::to_string(http_reply.result_int()));
            }
            
            std::string location = dispatcher->match(http_request);
            std::string uri = {http_request.target().begin(),http_request.target().end()}; //convert beast string view to string
            Request_Handler* handler = handler_factory->create(location, uri);
            if (handler == nullptr) {
                //If handler is a nullptr then a handler is not found for the request and reply with a bad request
                //*******stock reply************
                http_reply.result(boost::beast::http::status::bad_request);
                const char bad_request[] =
                            "<html>"
                            "<head><title>Bad Request</title></head>"
                            "<body><h1>400 Bad Request</h1></body>"
                            "</html>\n";
                http_reply.body() = bad_request;
                http_reply.content_length(http_reply.body().size());
                http_reply.set(boost::beast::http::field::content_type, "text/html");
                //*******soick reply************
                server_logger->log_warning("Bad Request -- Request handler not found");
                server_logger->log_info("Response code: " + std::to_string(http_reply.result_int()));
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
            server_logger->log_info("Response code: " + std::to_string(http_reply.result_int()));
            if (http_reply.result() == boost::beast::http::status::ok) {
                server_logger->log_info("Good request");
            }
            //Bind the reply to the buffer associated with handle_write//
            boost::beast::http::async_write(
                socket_, http_reply,
                boost::bind(&session::handle_write, shared_from_this(),boost::asio::placeholders::error));
            server_logger->log_trace("session::handle_read: async_write");
        }
        //If the request is not a valid request i.e. result == false. Then we set the reply object to a default bad_request and write to buffer.
        else {
            server_logger->log_info("Bad request");

            // Create a response indicating that the request was bad
            
            //*******stock reply************
            http_reply.result(boost::beast::http::status::bad_request);
            const char bad_request[] =
                        "<html>"
                        "<head><title>Bad Request</title></head>"
                        "<body><h1>400 Bad Request</h1></body>"
                        "</html>\n";
            http_reply.body() = bad_request;
            http_reply.content_length(http_reply.body().size());
            http_reply.set(boost::beast::http::field::content_type, "text/html");
            //*******soick reply************
            boost::beast::http::async_write(
                socket_,http_reply,
                boost::bind(&session::handle_write, shared_from_this(),boost::asio::placeholders::error));
            server_logger->log_trace("session::handle_read: async_write");
        }

    }
    else {
        //If the there is an error reading from the socket then we log the information and delete the session object.
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