#include <iostream>
#include <map>
#include <vector>
#include <boost/assign/list_of.hpp> // for 'list_of()'
#include <boost/assign/std/vector.hpp> // for 'operator+=()'

using std::cout;
using std::cerr;
using std::endl;

int* show_memory_leak(){
    return new int[5];
}

void show_out_of_range(){
    using namespace boost::assign;
    std::vector<int> values;
    values += 1, 2, 3, 4, 5, 6, 7, 8, 9;// insert values at the end of the container    
    values[100] = 5;
}

void show_uninitialized_var(){
    bool check_me;
    if(check_me){
        cout << "I'm true!"<< endl;
    }
    else{
        cout << "false!"<< endl;
    }
}

void show_fpe(){
    
}

void show_cachemiss(){
  long val1 = 100;
  long telm01 = 0;
  long telm31 = 0;
  long telm21 = 0;
  long telm11 = 0;
  long elmg1[] = {1,5,2,6,3,2,4,66,7,4,9,0,3,7,5,0,1,4,2,3};
  for (size_t i = 0; i < 20; i++) {
    telm01 = elmg1[i];
    telm31 = (telm01 << 3) ^ val1;
    telm21 = (telm01 << 2) ^ (val1 >> 1);
    telm11 = (telm01 << 1) ^ (val1 >> 2);
  }
  cout << telm01 << ' ' << telm31 << ' ' << telm21 << ' ' << telm11 << endl;
}

enum operation{
    memleak,
    oor,
    uninit,
    fpe,
    cachemiss
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
        case cachemiss:
	    show_cachemiss();
	    break;
        default:
            return;
    }
}

void show_usage(){
  std::cout << "Usage: program memleak|oor|uninit|fpe|cache" << std::endl;
}

int main(int argc, char* argv[]) {


    std::map<std::string,operation> program_mode
            = boost::assign::map_list_of("memleak",memleak)
            ("oor",oor)
            ("uninit",uninit)
            ("fpe",fpe)
            ("cache", cachemiss);

    if (argc < 2) {
      show_usage();
      return 0;
    }
    try {
        typedef std::map<std::string, operation>::const_iterator map_cit;
        for (unsigned i = 1; i < argc; i++) {
            std::string mode(argv[i]);
            map_cit it = program_mode.begin();
            if ((it = program_mode.find(mode)) != program_mode.end()) {
                execute_example((*it).second);
            }
        }
    }
    catch(const std::exception& e){
        cout << e.what() << endl;
    }
    return 0;
}
