#include <cassert>
#include <iostream>

using namespace std;

// Task 1.12 From Satter 1
// Templates and typename

template <typename T>
struct BaseTypedef{
    typedef T instantiated_type;
};


template <>
struct BaseTypedef<char>{
    static const long instantiated_type = 1;
};

template <typename A>
struct UseTypedef : public BaseTypedef<A>{
    // without typename does not compiled because of specialization
    // BaseTypedef<char>, where 'instantiated_type' not a type, but static member
    // which is absolutely correct from syntax view
    typename BaseTypedef<A>::instantiated_type _instance;
};


// Example with typedef from base class typename
class Rose{};

struct A{
    typedef Rose rose;
};

template <typename T>
struct B : public T {
    typedef typename T::rose foo;
};

template <typename T>
void smell(T){
    std::cout << "Good!\n";
}

void smell(Rose){
    std::cout << "Bad!\n";
}

int main(){
    UseTypedef<int> use;

    // Passed type is Rose in both cases
    smell(A::rose());
    smell(B<A>::foo());
    return 0;
}

