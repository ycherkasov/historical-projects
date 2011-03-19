//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_service io_service;

    // resolves the server name that was specified as a parameter to the application
    tcp::resolver resolver(io_service);

    // resolver takes a query object and turns it into a list of endpoints
    tcp::resolver::query query(argv[1], "daytime");

    // The list of endpoints is returned using an iterator
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // A default constructed resolver::iterator object is used as the end iterator
    tcp::resolver::iterator end;

    // Now we create and connect the socket. 
    // The list of endpoints obtained above may contain both IPv4 and IPv6 endpoints, 
    // so we need to try each of them until we find one that works. 
    // This keeps the client program independent of a specific IP version. 
    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);

    // The connection is open now
    // we need to read the response from the daytime service
    for (;;)
    {
      boost::array<char, 128> buf;       // get response here
      boost::system::error_code error;   // error flag
      
      // When the server closes the connection, the socket::read_some() function 
      // will exit with the eof error, which exit the loop
      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

