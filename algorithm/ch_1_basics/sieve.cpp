#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include "sieve.h"

typedef std::vector<unsigned> array_t;

//#define SIMPLE

/*
Levitin book 1.1
Algorithm basics
Greater common divisor exercise
*/


/// Exercise 1.9
unsigned lagrest_multiple(unsigned num){
    double r = static_cast<double>(num);
    double sqrtr = floor(sqrt(r));
    return static_cast<unsigned>(sqrtr);
}

void init_simple(unsigned* arr, size_t siz){
    for(size_t i = 0 ; i < siz ; i++)
        arr[i] = i+1;
}

void sieve_simple(unsigned* arr, size_t siz){
    unsigned largest = lagrest_multiple(siz);
    arr[0] = 0;
    for(size_t i = 2 ; i <= largest ; i++){
        unsigned element = i - 1;
        unsigned step = i;
        if(arr[element]){
            while((element += step) < siz){
                arr[element] = 0;
            }
        }
    }
}

// todo : boost.assign
void init(unsigned range, array_t& out){
    unsigned r(range);
    out.reserve(r);
    for(size_t i = 1 ; i <= 25 ; i ++){
        out.push_back(i);
    }
}

void sieve(array_t& out){
    unsigned largest = lagrest_multiple(out.size());
}

int show_sieve()
{
    const size_t range = 25;
    
#ifdef SIMPLE
    unsigned arr[range] = {};
    init_simple(arr, range);
    sieve_simple(arr, range);
#else
    array_t arr;
    arr.reserve(range);
    init(range, arr);
    sieve(arr);
#endif
    return 0;
} 