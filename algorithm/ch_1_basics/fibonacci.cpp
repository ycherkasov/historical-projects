#include <iostream>
#include "fibonacci.h"

static const unsigned max_fibonacci_number = 20;

int _fibonacci_naive(int n){

    if(n <= 0)
        return 0;

    if(n == 1 || n == 2){
        return 1;
    }
    else{
        return _fibonacci_naive(n - 2) + _fibonacci_naive(n - 1);
    }
}

int _fibonacci_effective(int n){

    if(n <= 0)
        return 0;

    int next = 0;

    if(n == 1 || n == 2){
        return 1;
    }
    else{
        int cache[2] = {1,1};
        for(int i = 1 ; i < n-1; i++)
        {
            next = cache[0] + cache[1];
            cache[i % 2] = next;
        }
        return next;
    }
}

void fibonacci_naive(){
    for(int i = 1 ; i < max_fibonacci_number; i++)
    {
        std::cout << _fibonacci_naive(i) << ' ';
    }
    std::cout << std::endl;
}


void fibonacci_effective(){
    for(int i = 1 ; i < max_fibonacci_number; i++)
    {
        std::cout << _fibonacci_effective(i) << ' ';
    }
    std::cout << std::endl;
}

void fibonacci_metaprogramming()
{
    //Generate (at compile time) 20 places of the Fib sequence.
    //Then, (at runtime) output the 20 calculated places.
    //Note: a 64 bit int overflows at place 92
    for (int i = 1; i < max_fibonacci_number; ++i)
    {
        std::cout << Fib<max_fibonacci_number>::getValue(i) << ' ';
    }
    std::cout << std::endl;
}

