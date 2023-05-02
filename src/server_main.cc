//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <signal.h>

<<<<<<< Updated upstream
#include "server.h"
#include "config_parser.h"
#include "logger.h"
using boost::asio::ip::tcp;

// Signal handler to be able to log server termination
void sigHandler(int sig_num) {
  ServerLogger *server_logger = ServerLogger::get_server_logger();
  // log a warning termination, since termination might 
  // be due to unwanted reasons
  server_logger->log_warning("Server has terminated");
  exit(sig_num);
}

int main(int argc, char* argv[]) {
  // fetch logger instance
  ServerLogger *server_logger = ServerLogger::get_server_logger();
  try {
    // Handling signals to terminate server such as Ctrl+C 
    // so that we can log server termination
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = sigHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    if (argc != 2) {
      server_logger->log_fatal("Usage: async_tcp_echo_server <port>");
=======
#include "../include/server.h"
#include "../include/config_parser.h"
int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <port>\n";
>>>>>>> Stashed changes
      return 1;
    }

    boost::asio::io_service io_service;

    NginxConfigParser parser;
    NginxConfig config;

    server_logger->log_trace("Parsing config file");
    if (!parser.Parse(argv[1], &config)) {
      server_logger->log_fatal("Failed to parse config file");
      return 1;
    }
    int port = config.GetServerPort();
    if (port == -1) {
      server_logger->log_fatal("Failed to get port from config file");
      return 1;
    }
    server_logger->log_trace("Config file parsed");

<<<<<<< Updated upstream
    server s(io_service, port);
    server_logger->log_trace("Server started at port " + std::to_string(port));
=======
    server s(io_service, port,config);
    // std::cout << "Server started at port " << port << std::endl;

>>>>>>> Stashed changes
    io_service.run();
  }
  catch (std::exception& e) {
    server_logger->log_fatal("Exception: " + std::string(e.what()));
    // std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
