#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

void magic(int n){
  int r = n % 2;
  std::cout << r;
  if(n > 1){
    magic(n/2);
  }
}

/*
 * last 1-0
 * last 0 - even
 * last 1 - odd
 *
 *
 *
 */

int unmagic(std::string& magic_str){

}

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cout << "Usage: program <number>" << std::endl;
    }
    try{
        magic( boost::lexical_cast<int> (argv[1]) );
        std::cout << std::endl;
    }
    catch(boost::bad_lexical_cast& e){
        std::cerr << e.what() << "(not a number?)" << std::endl;
    }

    return 0;
}
