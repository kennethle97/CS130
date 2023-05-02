#include <boost/bind.hpp>
#include <boost/asio.hpp>

<<<<<<< Updated upstream
#include "server.h"
// #include "logger.h"
=======
#include "../include/server.h"
>>>>>>> Stashed changes

server::server(boost::asio::io_service& io_service, short port,const NginxConfig &config)
: io_service_(io_service),
<<<<<<< Updated upstream
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}


void server::start_accept() {
    session* new_session = new session(io_service_);
=======
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    dispatcher(std::make_shared<Request_Handler_Dispatcher>(config))
{
    auto d_session = std::make_shared<session>(io_service_, dispatcher);
    start_accept(*d_session);
}


void server::start_accept(session& d_session)
{
    auto new_session = std::make_shared<session>(io_service_,dispatcher);
>>>>>>> Stashed changes
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session.get(),
            boost::asio::placeholders::error));
}

void server::handle_accept(session* new_session,
    const boost::system::error_code& error) {
    if (!error) {
        new_session->start();
    }
    else {
        delete new_session;
    }

    start_accept(*new_session);
}