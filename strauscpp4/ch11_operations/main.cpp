#include <iostream>

using namespace std;

/*

New features:
* Lambdas (3.4.3,11.4)
* Local classes as template arguments (11.4.2, 25.2.1)


Examples:
1. Initializer list
2. Lambda

*/

void show_initializer_list() {
    // The basic idea of initializer lists as expressions is that if you can initialize a variable x using the notation
    int x{ 1 };

    //A unqualified list is used where an expected type is unambiguously known.It can be used as an expression only as :
    //* A function argument
    //* A return value
    //* The right - hand operand of an assignment operator (=, +=, *=, etc.)
    //* A subscript

    // When used as the initializer for a named object without the use of a = (as for v above), 
    // an unqualified{}-list performs direct initialization(16.2.6)
    // In all other cases, it performs copy initialization(16.2.6)
}

/*
Unfortunately, we do not deduce the type of an unqualified list for a plain template argument

template<typename T> void f(T);

f({}); // error: type of initializer is unknown
f({ 1 }); // error: an unqualified list does not match ‘‘plain T’’ 
f({1,2}); // error: an unqualified list does not match ‘‘plain T’’

this is a language restriction, rather than a fundamental rule
*/

void show_lambda() {
    
    // capture lists

    // Naming the lambda is often a good idea

    // If you need to capture a variadic template (28.6) argument, use ...

    // In the unlikely event that we want to modify the state, we can declare the lambda mutable

    // we can use it to initialize a variable declared auto or std::function<R(AL)>
    // where R is the lambda’s return type and AL is its argument list
}

int main() {
    return 0;
}
