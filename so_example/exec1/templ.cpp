#include <iostream>
#include <boost/lexical_cast.hpp>
#include "shared1/wrapper.h"

// Use shared library as a wrapper under static1-2

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cout << "Usage: program <number>" << std::endl;
    }
    try{
      show_extern();
      c_function1();
    }
    catch(const boost::bad_lexical_cast& e){
    }
    catch(const std::exception& e){
    }
    return 0;
}
