#include <boost/asio.hpp>

#include "session.h"
#include "request_handler_dispatcher.h"

using boost::asio::ip::tcp;

class server
{
public:
  server(boost::asio::io_service& io_service, short port,const NginxConfig &config);

  // using friend class that has functions to take in pointer to server class
  // and then use that server pointer to call its private functions seems to be
  // only way I can think of currently to test private methods.
  friend class testserver;

private:
  void start_accept();

  void handle_accept(std::shared_ptr<session> new_session,
      const boost::system::error_code& error);

  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  std::shared_ptr<Request_Handler_Dispatcher> dispatcher;
};

class testserver {
  public:
    void friend_start_accept(server* serv) {
      serv->start_accept();
    }

    void friend_handle_accept(server* serv, std::shared_ptr<session> new_session, const boost::system::error_code& error) {
      serv->handle_accept(new_session, error);
    }

};