#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

#include "session.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service)
    : socket_(io_service)
{
}

tcp::socket& session::socket()
{
    return socket_;
}

void session::start()
{
   boost::asio::async_read_until( socket_, 
        boost::asio::dynamic_buffer(data_),
        "\r\n\r\n",
        boost::bind(&session::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
{
    if (!error)
    {
      std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
      std::string full_response = header + data_;
      std::cout << full_response << std::endl;
      boost::asio::async_write(socket_,
          boost::asio::buffer(full_response.c_str(), full_response.length()),
          boost::bind(&session::handle_write, this,
            boost::asio::placeholders::error));
    }
    else
    {
      delete this;
    }
}


void session::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
    boost::system::error_code shutdown_error;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, shutdown_error);
    // session::reset_data();
    // boost::asio::async_read_until(socket_,
    //  boost::asio::dynamic_buffer(data_),
    //       "\r\n\r\n",
    //       boost::bind(&session::handle_read, this,
    //       boost::asio::placeholders::error,
    //       boost::asio::placeholders::bytes_transferred));
    // }
    }
    else
    {
      delete this;
    }
}