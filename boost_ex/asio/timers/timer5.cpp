//
// timer5.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class printer
{
public:
  printer(boost::asio::io_service& io)
    : strand_(io),
      timer1_(io, boost::posix_time::seconds(1)),
      timer2_(io, boost::posix_time::seconds(1)),
      count_(0)
  {
    // running two timers in parallel
    // each callback handler is "wrapped" using the boost::asio::strand object
    // we are ensuring that they cannot execute concurrently
    timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
    timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << "\n";
  }

  // the shared resources used by the handlers (print1 and print2) 
  // are std::cout  and the count_ data member
  void print1()
  {
    if (count_ < 10)
    {
      std::cout << "Timer 1: " << count_ << "\n";
      ++count_;

      std::cout << "Expires at " << timer1_.expires_at() + boost::posix_time::seconds(1)  << "\n";
      timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
      std::cout << "Wait until print2() finishes..." << "\n";
      timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
    }
  }

  void print2()
  {
    if (count_ < 10)
    {
      std::cout << "Timer 2: " << count_ << "\n";
      ++count_;

      std::cout << "Expires at " << timer2_.expires_at() + boost::posix_time::seconds(1)  << "\n";
      timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
      std::cout << "Wait until print1() finishes..." << "\n";
      timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
    }
  }

private:
  boost::asio::strand strand_;
  boost::asio::deadline_timer timer1_;
  boost::asio::deadline_timer timer2_;
  int count_;
};

int main()
{
  boost::asio::io_service io;
  printer p(io);

  // io_service::run()  to be called from two threads: 
  // the main thread and one additional thread. 
  // This is accomplished using an boost::thread object. 
  boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
  io.run();

  // The background thread will not exit 
  // until all asynchronous operations have completed. 
  t.join();

  return 0;
}

