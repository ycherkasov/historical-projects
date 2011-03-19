//
// timer44.cpp
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

class printer
{
public:
  printer(boost::asio::io_service& io)
    : timer_(io, boost::posix_time::seconds(1)),
      count_(0)
  {
    timer_.async_wait(boost::bind(&printer::print, this));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << "\n";
  }

  void print()
  {
    if (count_ < 5)
    {
      std::cout << count_ << "\n";
      ++count_;

      // as same as timer3, but use function member instead
      std::cout << "Expires at " << timer_.expires_at() + boost::posix_time::seconds(1)  << "\n";
      timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
      std::cout << "Wait..." << "\n";
      timer_.async_wait(boost::bind(&printer::print, this));
    }
  }

private:
  boost::asio::deadline_timer timer_;
  int count_;
};


// identically to the tutorial program Timer.3.
// use a class member function as a callback handler
int main()
{
  boost::asio::io_service io;
  // The constructor of this class will take a reference to the io_service object 
  // and use it when initialising the timer_  member.
  printer p(io);
  std::cout << "Run IO" << "\n";
  io.run();

  return 0;
}

