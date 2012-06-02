#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include "app.h"


int main(int argc, char **argv) {
    app a;
    try {
	a.parse_command_line(argc, argv);
	a.start();
    }
    catch(const std::exception& e){
	std::cout << e.what() << std::endl;
    }
    
    return 0;
}