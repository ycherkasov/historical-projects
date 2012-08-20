#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include "integr.h"

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cout << "Usage: program <number>" << std::endl;
        
    }
    try{
        int thread_cout = boost::lexical_cast<int>(argv[1]);
        for (int i = 0; i < thread_cout; i++ ){
            boost::thread t;
        }
    }
    catch(const boost::bad_lexical_cast& e){
    }
    catch(const std::exception& e){
    }
    return 0;
}
