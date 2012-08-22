// M_PI
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/random/lagged_fibonacci.hpp>
#include <boost/random/uniform_real_distribution.hpp>
// todo: replace with more concrete headers
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "integr.h"


#define _MULTITHREAD

using std::cout;
using std::cerr;
using std::endl;

typedef double (*function_t)(double);

double f_sin(double x){
    return sin(x);
}

bool sub_function(function_t func, double x, double y){
    double real_y = func(x);
    bool res = true;
    if(real_y < 0){
        res = ((y < 0) && (y > real_y));
    }
    else if(real_y > 0){
        res = ((y > 0) && (y < real_y));
    }
    return res;
}

void usage(){
    cerr << "Usage: program <number>" << endl;
    exit(0);
}

double rectangle_integrate(double a, double b, size_t steps, function_t func){
    
    // todo: close enough
    if(a == b){
        return 0;
    }

    double begin = (b>a)?a:b;
    double end = (b>a)?b:a;

    double interval = (end - begin)/steps;
    double half_int = interval/2;
    double result = 0.;
    
    for(size_t i = 1 ; i < steps; i++){
        double point = begin + interval*i - half_int;
        result += func(point);
    }
    result *= interval;
    return result;
}

double trapeziodal_integrate(double a, double b, size_t steps, function_t func){
        // todo: close enough
    if(a == b){
        return 0;
    }

    double begin = (b>a)?a:b;
    double end = (b>a)?b:a;

    double interval = (end - begin)/steps;

    double result = (func(begin) + func(end))/2;

    for(size_t i = 1 ; i < (steps-1); i++){
        double point = begin + interval*i;
        result += func(point);
    }
    result *= interval;
    return result;
}

double simpson_integrate(double a, double b, size_t steps, function_t func){
        
    // todo: close enough
    if(a == b){
        return 0;
    }

    double begin = (b>a)?a:b;
    double end = (b>a)?b:a;

    double interval = (end - begin)/steps;
    double h = interval/6;

    double result = 0.;
    double coeff1 = 0.;
    double coeff2 = 0.;

    for(size_t i = 0 ; i < steps; i++){
        // x(k)
        double x_k = begin + interval*(i+1);
        // x(k-1)
        double x_k_1 = begin + interval*i;
        double point1 = x_k;
        double point2 = (x_k_1 + x_k)/2;
        coeff1 += func(point1);
        coeff2 += func(point2);
    }
    coeff1 -= func(end);
    result = h * (func(begin) + func(end) + 2*coeff1 + 4*coeff2);
    return result;
}

double monte_carlo_integrate(double a, double b, double up, double low, function_t func){
        // todo: close enough
    if((a == b) || (up == low)){
        return 0;
    }

    
    double begin = (b>a)?a:b;
    double end = (b>a)?b:a;
    size_t in_function = 0;
    size_t total = 100000;
    double square = (end-begin)*(up-low);

    boost::random::lagged_fibonacci3217 gen;
    for(size_t i = 0 ; i < total ; i++){
        boost::random::uniform_real_distribution<> dist1(begin, end);
        boost::random::uniform_real_distribution<> dist2(low, up);
        double x = dist1(gen);
        double y = dist2(gen);
        
        if(sub_function(func, x, y)) {
            in_function++;
        }
    }


    double result = static_cast<double>(in_function)/total;
    return result*square;
}

boost::mutex m;
volatile double result = 0;

template <typename T>
void interlocked_add(volatile T& val, const T add){
    boost::mutex::scoped_lock l(m);
    val += add;
}

void threaded_integrate(double a, double b, size_t steps, function_t func){
    //double subresult = rectangle_integrate(a, b, steps, func);
    //double subresult = trapeziodal_integrate(a, b, steps, func);
    double subresult = simpson_integrate(a, b, steps, func);
    interlocked_add<double>(result, subresult);
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
        const size_t tasks = 10;
        const size_t steps = 100000;
        double begin = 0.;
        double end = M_PI;
        double interval = (end - begin)/tasks;


        boost::asio::io_service io;
        boost::asio::io_service::work asio_work(io);
        boost::thread_group th_group;
        for (size_t i = 0; i < thread_count; ++i){
            th_group.create_thread(boost::bind(&boost::asio::io_service::run, &io));
        }

        for(int i = 0 ; i < tasks; i++){
            double x0 = begin + (interval*i);
            double x1 = begin + (interval*(i+1));
            cout << "x" << i << " = " << x0 << ", x" << i+1 << " = " << x1 << endl;
            io.post(boost::bind(threaded_integrate, x0, x1, steps/tasks, f_sin));
        }
        io.stop();
        th_group.join_all();
        cout << result << endl;
    
        result = rectangle_integrate(0., M_PI, 100000, f_sin);
        cout << result << endl;

        result = trapeziodal_integrate(0., M_PI, 100000, f_sin);
        cout << result << endl;

        result = simpson_integrate(0., M_PI, 100000, f_sin);
        cout << result << endl;

        result = monte_carlo_integrate(0., M_PI, 1.0, -1.0, f_sin);
        cout << result << endl;
    }
    catch(const std::exception& e){
        cerr << "Exception in program: " << e.what() << endl;
    }
    catch(...){
        cerr << "Generic exception" << endl;
    }
    return 0;
}
