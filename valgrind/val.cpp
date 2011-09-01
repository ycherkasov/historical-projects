#include <iostream>
#include <boost/lexical_cast.hpp>


void show_memory_leak(){}

void show_out_of_range(){}

void show_uninitialized_var(){}

void show_fpe(){}

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
