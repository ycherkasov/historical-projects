
#include <iostream>

using namespace std;

// Task 2.01-2.10 From Satter 1
// Develop exception-safe stack template

// First version of exception-safe stack
// It provides strong exception guarantee
// so it stays unchanged is case of thrown an exception
template <typename T>
class safe_stack{
public:
    safe_stack(size_t initial_size = 0);
    ~safe_stack();
private:

    // pointer to allocated memory
    T* _v;

    // number of real elements
    size_t _vsize;

    // capacity of allocated memory
    size_t _vcapacity;
};


//////////////////////////////////////////////////////////////////////////
// Implementation

// Constructor is completely exception safe
// 1. If _v allocated successfully, everything is ok
// 2. If operator new() throws an exception, memory is deallocated, objects is not created, bad_alloc is passed up to call stack
// 3. If T::T() throws an exception, memory is deallocated, objects is not created, bad_alloc is passed up to call stack
template<typename T>
safe_stack<T>::safe_stack(size_t initial_size/* = 0*/)
    : _v(initial_size ? new T[initial_size] : nullptr)
    , _vsize(0)
    , _vcapacity(initial_size){}

// Destructor could not throw exceptions
template<typename T>
safe_stack<T>::~safe_stack(){
    delete[] _v;
}


int main(){
    return 0;
}
