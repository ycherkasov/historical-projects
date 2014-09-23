#include <cassert>
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

    //////////////////////////////////////////////////////////////////////////
    // construction and destruction - exception-safe

    // Constructor is completely exception safe
    // 1. If _v allocated successfully, everything is ok
    // 2. If operator new() throws an exception, memory is deallocated, objects is not created, bad_alloc is passed up to call stack
    // 3. If T::T() throws an exception, memory is deallocated, objects is not created, bad_alloc is passed up to call stack
    safe_stack(size_t initial_size = 0)
        : _v(initial_size ? new T[initial_size] : nullptr)
        , _vsize(0)
        , _vcapacity(initial_size){}

    // Destructor could not throw exceptions
    ~safe_stack(){
        delete[] _v;
    }

    //////////////////////////////////////////////////////////////////////////
    // copy constructor and assignment

    // service function where working with memory is encapsulated
    // 1. in case of new() throws an exception memory is not allocated, exception is passed
    // 2. In case of throwing exception on copy it is caught and memori is released
    T* new_copy(const T* src, size_t src_size, size_t dst_size){
        assert(dst_size > src_size);
        T* dest = new T[dst_size];

        try{
            copy(src, src + src_size, dest);
        }
        catch (const std::exception& e){
            delete[] dest;
            throw;
        }
        return dest;
    }

    // Copy constructor based on new_copy
    // Just use new_copy(), it handles all exceptions
    safe_stack(const safe_stack& rhs)
        : _v(new_copy(rhs._v, rhs._vsize, rhs._vcapacity))
        , _vsize(rhs._vsize)
        , _vcapacity(rhs._vcapacity){}

    // Assignment operator based on new_copy
    // Just use new_copy(), it handles all exceptions
    safe_stack& operator=(const safe_stack& rhs){
        if (this != &rhs){
            T* v_new = new_copy(rhs._v, rhs._vsize, rhs._vcapacity);
            delete[] _v;
            _vsize = rhs._vsize;
            _vcapacity = rhs._vcapacity;
        }
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    // Stack methods
    size_t count() const {
        return _vsize;
    }

    void push(const T& t){
        if (_vsize == _vcapacity) {
            size_t new_capacity = _vcapacity ? _vcapacity * 2 + 1 : 16; // 16 is minimal initial value
            T* v_new = new_copy(_v, _vsize, new_capacity);
            delete[] _v;
            _v = v_new;
            _vcapacity = new_capacity;
        }
        _v[_vsize] = t;
        ++_vsize;
    }
#if 0
    T pop(){
        if (_vsize == 0){
            throw std::runtime_error("Pop from empty stack");
        }
        else{
            T result = _v[_vsize - 1];
            --_vsize;
            return result;
        }
    }
#else
    void pop(){
        if (_vsize == 0){
            throw std::runtime_error("Pop from empty stack");
        }
        else{
            --_vsize;
        }
    }

    T& top(){
        if (_vsize == 0){
            throw std::runtime_error("Top from empty stack");
        }
        else{
            return _v[_vsize - 1];
        }
    }

    const T& top() const {
        if (_vsize == 0){
            throw std::runtime_error("Top from empty stack");
        }
        else{
            return _v[_vsize - 1];
        }
    }
#endif
private:

    // pointer to allocated memory
    T* _v;

    // number of real elements
    size_t _vsize;

    // capacity of allocated memory
    size_t _vcapacity;
};


/////////////////////////////////////////////
void show_safe_stack1(){
    safe_stack<int> ss;
    ss.push(1);
    ss.push(2);
    size_t sz = ss.count();
    // Unsafe!
    // int i = ss.pop();
    int i = ss.top();
    ss.pop();
}

int main(){
    show_safe_stack1();
    return 0;
}
