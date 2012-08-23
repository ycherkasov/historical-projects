// M_PI
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>

#include "integr.h"


#define _MULTITHREAD

using std::cout;
using std::cerr;
using std::endl;

class clock_count {
public:
	clock_count(){
		_clock_count = GetTickCount();
	}
	~clock_count(){
		_clock_count = GetTickCount() - _clock_count;
        std::cout << "Timer: " << _clock_count << std::endl;
	}

private:
	double _clock_count;
};

double f_sin(double x){
    return sin(x);
}

void usage(){
    cerr << "Usage: program <number>" << endl;
    exit(0);
}

void numerical_integration(size_t thread_count){

    double result = 0.;
    const size_t steps = 1000000;

    // S-threaded
    num_integrte nint(f_sin);

    {
        clock_count c;
        result = nint.rectangle_integrate(0., M_PI, steps);
    }
    cout << result << endl;

    {
        clock_count c;
        result = nint.trapeziodal_integrate(0., M_PI, steps);
    }
    cout << result << endl;

    {
        clock_count c;
        result = nint.simpson_integrate(0., M_PI, steps);
    }
    cout << result << endl;

    {
        monte_carlo mc(f_sin);
        result = mc.monte_carlo_integrate(0., M_PI, 1.0, -1.0);
    }
    cout << result << endl;

    // M-Threaded

    num_integrte_mt int_mt(f_sin, thread_count);

    {
        clock_count c;
        int_mt.mt_integrate(0., M_PI, steps, 10);
    }
    cout << result << endl;
}

int main(int argc, char* argv[]) {
    
    size_t thread_count = 6;

#if 0
    if (argc != 2) {
        usage();
    }
    
    try{
        thread_count = boost::lexical_cast<int>(argv[1]);
    }
    catch(const boost::bad_lexical_cast& e){
        cerr << e.what() << endl;
        usage();
    }
#endif

    try{

        numerical_integration(thread_count);

    }
    catch(const std::exception& e){
        cerr << "Exception in program: " << e.what() << endl;
    }
    catch(...){
        cerr << "Generic exception" << endl;
    }
    return 0;
}
