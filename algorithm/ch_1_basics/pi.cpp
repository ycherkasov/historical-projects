//#include <boost/random/normal_distribution.hpp>
//#include <boost/random/mersenne_twister.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "pi.h"

// calculate pi - Monte-carlo
bool inCircle(double x, double y) {
    return x*x + y*y < 1;
}

//double monte_carlo_pi(size_t n) {
//    boost::uniform_01<boost::mt19937> gen((boost::mt19937()));
//    size_t inside = 0;
//
//    for (size_t i = 0; i < n; i++)
//        inside += inCircle(gen(), gen());
//
//    return 4.0 * inside / n;
//}

// calculate pi - Gregory formula (Gregory–Leibniz series)
// http://en.wikipedia.org/wiki/Pi
double gregori_pi(double delta){
    if(delta >= 1.)
        throw std::range_error("Delta too big");

    // first approximation
    double pi = 0.0;
    double pi_prev = 4.0;

    // first divisor
    double step = 1.0;
    int sign = -1;
    while(delta < abs(pi - pi_prev)){
        sign *= (-1);
        pi_prev = pi;
        pi += sign * (4.0/step);
        step += 2;
    }
    return pi;
}

// calculate pi - Gauss formula
double gouble_gauss_pi(){
    double pi = 0.0;
    return pi;
}


void show_pi(){
    std::cout << gregori_pi(10e-5) << std::endl;
}