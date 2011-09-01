//
// timer3.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


// To implement a repeating timer using asio you need to change 
// the timer's expiry time in your callback function
// this means that the callback function will need to access the timer object (param t)
void print(const boost::system::error_code& /*e*/,
    boost::asio::deadline_timer* t, int* count)
{
  if (*count < 5)
  {
    std::cout << *count << "\n";
    ++(*count);

    std::cout << "Expires at " << t->expires_at() + boost::posix_time::seconds(1)  << "\n";
    t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
    std::cout << "Wait..." << "\n";
    // call print() with async_wait() recursively
    // we move the expiry time for the timer along by one second from the previous expiry time
    t->async_wait(boost::bind(print,
          boost::asio::placeholders::error, t, count));
  }
}

int main()
{
  boost::asio::io_service io;

  int count = 0;
  std::cout << "Init timer" << "\n";
  boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
  t.async_wait(boost::bind(print,
        boost::asio::placeholders::error, &t, &count));

  std::cout << "Run IO" << "\n";
  io.run();

  std::cout << "Final count is " << count << "\n";

  return 0;
}

