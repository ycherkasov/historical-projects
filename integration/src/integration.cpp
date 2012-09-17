// M_PI
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "integration.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <time.h>
#endif

#define _MULTITHREAD

using std::cout;
using std::cerr;
using std::endl;

class clock_count {
public:
    clock_count(){
#ifdef WIN32
        _clock_count = GetTickCount();
#else
        _clock_count = clock() / (CLOCKS_PER_SEC / 1000);
#endif
    }

    ~clock_count(){
#ifdef WIN32
        _clock_count = GetTickCount() - _clock_count;
#else
        _clock_count = clock() / (CLOCKS_PER_SEC / 1000) - _clock_count;
#endif
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

void numerical_integration(){

    cout << "Single-threaded versions" << endl;
    double result = 0.;
    const size_t steps = 1000000;

    // S-threaded
    num_integrte nint(f_sin);

    cout << "Rectangular:" << endl;
    {
        clock_count c;
        result = nint.rectangle_integrate(0., M_PI, steps);
    }
    cout << "Result: " << result << endl << endl;

    cout << "Trapezoidal:" << endl;
    {
        clock_count c;
        result = nint.trapezoidal_integrate(0., M_PI, steps);
    }
    cout << "Result: " << result << endl << endl;

    cout << "Simpson:" << endl;
    {
        clock_count c;
        result = nint.simpson_integrate(0., M_PI, steps);
    }
    cout << "Result: " << result << endl << endl;




}

void threaded_numerical_integration(size_t thread_count){
    
    // M-Threaded
    cout << "Multi-threaded versions" << endl;
    double result = 0.;
    const size_t steps = 1000000;


    num_integrte_mt nint(f_sin, thread_count);

    cout << "Rectangular:" << endl;
    {
        clock_count c;
        result = nint.rectangle_integrate(0., M_PI, steps);
    }
    cout << "Result: " << result << endl << endl;

    cout << "Trapezoidal:" << endl;
    {
        clock_count c;
        result = nint.trapezoidal_integrate(0., M_PI, steps);
    }
    cout << "Result: " << result << endl << endl;

    cout << "Simpson:" << endl;
    {
        clock_count c;
        result = nint.simpson_integrate(0., M_PI, steps);
    }
    cout << "Result: " << result << endl << endl;

}

void monte_carlo_numerical_integration(){
    
    double result = 0.;
    {
        clock_count c;
        monte_carlo mc(f_sin);
        result = mc.monte_carlo_integrate(0., M_PI, 1.0, -1.0);
    }
    cout << "Result: " << result << endl << endl;
}

int main(int argc, char* argv[]) {

    const size_t thread_count = 8;
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

        numerical_integration();
        threaded_numerical_integration(thread_count);

    }
    catch(const std::exception& e){
        cerr << "Exception in program: " << e.what() << endl;
    }
    catch(...){
        cerr << "Generic exception" << endl;
    }
    return 0;
}