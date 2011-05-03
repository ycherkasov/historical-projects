#include <iostream>
#include <stdexcept>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 12 Stievens, Rago. 
 *
 */


int main(int argc, char* argv[]) {


    try {
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
