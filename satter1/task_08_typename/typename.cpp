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
    typename BaseTypedef::instantiated_type _instance;
};


int main(){
    UseTypedef<int> use;
    return 0;
}

