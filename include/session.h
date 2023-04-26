#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session
{
public:
  session(boost::asio::io_service& io_service);

  tcp::socket& socket();

  void start();

  friend class testsession;

private:
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);

  void handle_write(const boost::system::error_code& error);

  tcp::socket socket_;
  enum { max_length = 1024 };
  std::string data_;
};

class testsession {
  public:
  void friend_handle_read(session* sess, const boost::system::error_code& error,
      size_t bytes_transferred) {
        sess->handle_read(error, bytes_transferred);
      }
  
  void friend_handle_write(session* sess, const boost::system::error_code& error) {
    sess->handle_write(error);
  }
};