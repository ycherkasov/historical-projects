#include <iostream>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cout << "Usage: program <number>" << std::endl;
    }
    try{
    }
    catch(const boost::bad_lexical_cast& e){
    }
    catch(const std::exception& e){
    }
    return 0;
}
