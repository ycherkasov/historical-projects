//
// timer2.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

/*
Using asio's asynchronous functionality means having a callback function 
that will be called when an asynchronous operation completes. 
In this program we define a function called print  
to be called when the asynchronous wait finishes. 
*/
void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!\n";
}

int main()
{
  boost::asio::io_service io;

  boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
  /*
  Next, instead of doing a blocking wait, we call the async_wait() function to perform an asynchronous wait. 
  When calling this function we pass the print() callback
  */
  t.async_wait(print);

  std::cout << "Run io\n";
  //It is important to remember to give the io_service some work to do before calling io_service::run(). 
  //For example, if we had omitted the above call to deadline_timer::async_wait(), 
  //the io_service would not have had any work to do, and would have returned immediately. 
  io.run();

  std::cout << "Exit\n";
  return 0;
}

