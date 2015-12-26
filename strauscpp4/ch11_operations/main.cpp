#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

/*

New features:
* Lambdas (3.4.3,11.4)
* Local classes as template arguments (11.4.2, 25.2.1)


Examples:
1. Bitwise operations
2. Initializer list
3. Lambda
*/


void show_bitwise() {

    int* p = nullptr;
    while (p && !(*p)) ++p;
    // Here, p is not dereferenced if it is the nullptr

    // For example, one could extract the middle 16 bits of a 32 - bit int like this
    static_assert(sizeof(int) == 4, "unexpected int size"); 
    static_assert(sizeof(short) == 2, "unexpected short size");
}

int show_initializer_list() {

    // The basic idea of initializer lists as expressions is that if you can initialize a variable x using the notation
    int x{ 1 };

    // Qualified by a type, T{...}, meaning 'create an object of type T initialized by T{...}'
    int i1{ 42 };
    int* i2 = new int{ 42 };
    delete i2;

    // Unqualified {...}, for which the the type must be determined from the context of use

    //A unqualified list is used where an expected type is unambiguously known.It can be used as an expression only as :
    //* A function argument
    //* A return value
    //* The right - hand operand of an assignment operator (=, +=, *=, etc.)
    //* A subscript

    // When used as the initializer for a named object without the use of a = (as for v above), 
    // an unqualified{}-list performs direct initialization(16.2.6)
    // In all other cases, it performs copy initialization(16.2.6)

    // initializer (direct initialization)
    int v{ 7 };
    
    // initializer (copy initialization)
    int v2 = { 7 };
    
    // assume m takes value pairs
    std::pair<int, int> m = {2, 3};

    // overloaded (!) subscript
    std::map<int, int> arr;
    v = arr[{0}];

    // right-hand operand of assignment
    v = { 8 };

    // right-hand operand of assignment (?)
    // error C3079: an initializer list cannot be used as the right operand
    //v += {88};

    // error: not left-hand operand of assignment
    // This is not an insurmountable problem, but it was decided not to extend C++ in that direction
    // {v} = 9;

    // error: not an operand of a non-assignment operator
    // v = 7 + {10};

    // return value
    return{ 11 };

}

/*
Unfortunately, we do not deduce the type of an unqualified list for a plain template argument

template<typename T> void f(T);

f({}); // error: type of initializer is unknown
f({ 1 }); // error: an unqualified list does not match ‘‘plain T’’ 
f({1,2}); // error: an unqualified list does not match ‘‘plain T’’

this is a language restriction, rather than a fundamental rule
*/

void show_initializer_auto() {
    
    // The type of a{}-list can be deduced(only) if all elements are of the same type
    // and at least one element present
    
    //auto x0 = {}; 
    
    // initializer_list<int>:
    auto x1 = { 1 }; 
    auto x2 = { 1,2 }; 
    auto x3 = { 1,2,3 };

    //auto x4 = { 1,2.0 };
}

#if 0
namespace cpp4 {

template<typename T>
T adder(T v) {
  return v;
}

template<typename... Var> 
void algo(int s, Var... v) {

    auto helper = [&s, &v...](int i){ std::cout << s * i * adder(v...) << std::endl; };
    std::vector<int> v = { 1,2,3,4,5,6 };
    std::for_each(v.begin(), v.end(), helper);
}

}
#endif

void show_lambda() {

    int val1 = 42;
    int val2 = 666;
    std::vector<int> v = { 1,2,3,4,5,6 };
    
    // capture lists:

    // no capture
    std::for_each(v.begin(), v.end(), [](int i) {
        std::cout << i << std::endl;
    });

    // capture all by ref
    std::for_each(v.begin(), v.end(), [&](int i) {
        std::cout << i + val1 << std::endl;
    });

    // capture all by val
    std::for_each(v.begin(), v.end(), [=](int i) {
        std::cout << i + val2 << std::endl;
    });

    // capture one by ref
    std::for_each(v.begin(), v.end(), [&val1](int i) {
        std::cout << i + val1 << std::endl;
    });

    // capture one by val
    std::for_each(v.begin(), v.end(), [val2](int i) {
        std::cout << i + val2 << std::endl;
    });

    // capture list by ref
    std::for_each(v.begin(), v.end(), [&, val1, val2](int i) {
        std::cout << i + val1 + val2 << std::endl;
    });

    // capture list by val
    std::for_each(v.begin(), v.end(), [val1, val2](int i) {
        std::cout << i + val1 + val2 << std::endl;
    });

    // this is captured only by value

    // Naming the lambda is often a good idea
    auto l = [val1, val2](int i) {
        std::cout << i + val1 + val2 << std::endl;
    };
    std::for_each(v.begin(), v.end(), l);

    // If you need to capture a variadic template (28.6) argument, use ...
    // TODO: manage that: cpp4::algo(1, 2, 3, 4, 5, 6);

    // In the unlikely event that we want to modify the state, we can declare the lambda mutable
    size_t count = v.size();
    std::generate(v.begin(), v.end(), [count]() mutable {return --count; });
    // this is only for 'outer' state modification

    // we can use it to initialize a variable declared auto or std::function<R(AL)>
    // where R is the lambda’s return type and AL is its argument list:
    // recursive-lambda string reverse
    std::function<void(char* b, char* e)> rev = [&](char* b, char* e) { 
        if (1 < e-b) { swap(*b, *--e); rev(++b, e); } 
    };

    // A lambda might outlive its caller
    // This can happen if we pass a lambda to a different thread 
    // or if the callee stores away the lambda for later use
    // Think of the capture list as the initializer list for the closure object and [=] and [&]

    // A lambda that captures nothing can be assigned to a pointer to function of an appropriate type. For example
    double (*p1)(double) = [](double a) { return sqrt(a); };
}

int main() {
    show_bitwise();
    show_initializer_list();
    show_initializer_auto();
    show_lambda();
    return 0;
}
