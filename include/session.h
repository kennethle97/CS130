#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include "request_handler_dispatcher.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

using boost::asio::ip::tcp;
using request = boost::beast::http::request<boost::beast::http::string_body>;
using reply = boost::beast::http::response<boost::beast::http::string_body>;
using request_parser = boost::beast::http::request_parser<boost::beast::http::string_body>;

class session :public std::enable_shared_from_this<session>
{
public:
  session(boost::asio::io_service& io_service,
    std::shared_ptr<const Request_Handler_Dispatcher> dispatcher);

  tcp::socket& socket();

  void start();

  request http_request;
  reply http_reply;
  request_parser http_parser;
  
  friend class testsession;

private:
  void handle_read(std::shared_ptr<session> self,const boost::system::error_code& error,
      size_t bytes_transferred);

  void handle_write(const boost::system::error_code& error);

  tcp::socket socket_;
  enum { max_length = 1024 };
  boost::asio::streambuf data_;
  const std::shared_ptr<const Request_Handler_Dispatcher> dispatcher;
};

class testsession {
  public:
  void friend_handle_read(session* sess,std::shared_ptr<session> self, const boost::system::error_code& error,
      size_t bytes_transferred) {
        sess->handle_read(self,error, bytes_transferred);
      }
  
  void friend_handle_write(session* sess, const boost::system::error_code& error) {
    sess->handle_write(error);
  }
};

#endif