#include <iostream>
#include <map>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp> // for 'list_of()'
#include <boost/assign/std/vector.hpp> // for 'operator+=()'

int* show_memory_leak(){
    return new int[5];
}

void show_out_of_range(){
    using namespace boost::assign;
    std::vector<int> values;
    values += 1, 2, 3, 4, 5, 6, 7, 8, 9;// insert values at the end of the container    
    values[100] = 5;
}

void show_uninitialized_var(){}

void show_fpe(){}

enum operation{
    memleak,
    oor,
    uninit,
    fpe
};

void execute_example(operation op){
    switch(op){
        case memleak:
            show_memory_leak();
            break;
        case oor:
            show_out_of_range();
            break;
        case uninit:
            show_uninitialized_var();
            break;
        case fpe:
            show_fpe();
            break;
        default:
            return;
    }
}

int main(int argc, char* argv[]) {

    std::map<std::string,operation> program_mode
            = boost::assign::map_list_of("memleak",memleak)
            ("oor",oor)
            ("uninit",uninit)
            ("fpe",fpe);

    if (argc != 2) {
        std::cout << "Usage: program <mode>" << std::endl;
        
    }
    try {
        for (unsigned i = 1; i < argc; i++) {
            std::string mode(argv[i]);
            std::map<std::string, operation>::const_iterator it = program_mode.begin();
            if ((it = program_mode.find(mode)) != program_mode.end()) {
                execute_example((*it).second);
            }

        }
    }
    catch(const boost::bad_lexical_cast& e){
    }
    catch(const std::exception& e){
    }
    return 0;
}
