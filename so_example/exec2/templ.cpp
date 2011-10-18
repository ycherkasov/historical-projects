#include <iostream>
#include <boost/lexical_cast.hpp>
#include "static1/file1.h"
#include "static2/file2.h"


// use static libraries directly

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
