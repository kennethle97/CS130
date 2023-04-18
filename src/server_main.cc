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

#include "server.h"
#include "config_parser.h"

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    NginxConfigParser parser;
    NginxConfig config;

    if (!parser.Parse(argv[1], &config)) {
      std::cerr << "Failed to parse config file" << std::endl;
      return 1;
    }
    int port = config.GetServerPort();
    if (port == -1) {
      std::cerr << "Failed to get port from config file" << std::endl;
      return 1;
    }

    server s(io_service, port);
    // std::cout << "Server started at port " << port << std::endl;

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
