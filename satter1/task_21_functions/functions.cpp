#include <iostream>


// Task 8.1 Satter 1


// Create a function, that returns pointer to itself
// func_ptr_t p = func(); // called func()
// p(); // called func()

typedef int(*fp_int_double)(double);

int test1(double d){
    return static_cast<int>(d);
}


// State machine based on function pointers
/*
state_ptr_t start(const std::string& input);
state_ptr_t state2(const std::string& input);
state_ptr_t stop(const std::string& input);
state_ptr_t error(const std::string& input);
*/

// error: function returning finction is not allowed
//typedef state_ptr_t(*state_ptr_t)(const std::string&);

// Correct solution using Proxy pattern
class func_ptr_proxy;
typedef func_ptr_proxy(*func_ptr)();

struct func_ptr_proxy{

    // Accept holding function pointer
    func_ptr_proxy(func_ptr p) :_p(p){}

    // Cast to real function pointer
    operator func_ptr(){ return _p; }

private:
    func_ptr _p;
};

func_ptr_proxy test2(){ return test2; }


void show_function_pointer(){
    fp_int_double f = test1;
    int i = test1(1.5);
    func_ptr p = test2();
}

void show_macro(){

    // useful macro definitions
    std::cout << "File name: " << __FILE__ << std::endl;
    std::cout << "Code line number: " << __LINE__ << std::endl;
    std::cout << "Date of the compilation: " << __DATE__ << std::endl;
    std::cout << "Time of the compilation: " << __TIME__ << std::endl;

    // also debug/platform-dependent definitions
    // code concatenation, returning function name literal etc.
}

int main(){

    show_function_pointer();

    show_macro();

    return 0;
}
