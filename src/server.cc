#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "server.h"
// #include "logger.h"

server::server(boost::asio::io_service& io_service, short port,const NginxConfig &config)
    : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    dispatcher(std::make_shared<Request_Handler_Dispatcher>(config))
{
    start_accept();
}


void server::start_accept()
{
    auto new_session = std::make_shared<session>(io_service_,dispatcher);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
            boost::asio::placeholders::error));
}

void server::handle_accept(std::shared_ptr<session> new_session,
    const boost::system::error_code& error) {
    if (!error) {
        new_session->start();
    }

    start_accept();
}