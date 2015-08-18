#include <cassert>
#include <iostream>
#include <memory>

using namespace std;

// Task 2.01-2.10 From Satter 1
// Develop exception-safe stack template

// First version of exception-safe stack
// It provides strong exception guarantee
// so it stays unchanged is case of thrown an exception
// Requirements to T: default constructor, copy constructor, exception-safe operator=
// THese requirements could be reduced using the second version of stack
template <typename T>
class safe_stack{
public:

    //////////////////////////////////////////////////////////////////////////
    // construction and destruction - exception-safe

    // Constructor is completely exception safe
    // 1. If _v allocated successfully, everything is ok
    // 2. If operator new() throws an exception, memory is deallocated, objects is not created, bad_alloc is passed up to call stack
    // 3. If T::T() throws an exception, memory is deallocated, objects is not created, exception is passed up to call stack
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
    // 2. In case of throwing exception on copy it is caught and memory is released
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

    // Nothing bad could happen here
    size_t count() const {
        return _vsize;
    }

    // new_copy() may thown the exception, it will leave object in the old state
    // _v[_vsize] = t may thrown the exception, it will leave object in consistent state
    // (new object won't be pushed)
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
    // Could not be made exception-safe
    // In case of exception while returning by value (copy operatoin)
    // object would be destroyed and lost forever (both in stack and caller function)
    // So, does not exists exception-safe way to pop object atomically
    // Moreover, any mutator not to return object by value
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
    // So, split pop() operation into two
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

//////////////////////////////////////////////////////////////////////////
// Second version of exception-safe stack
// It provides strong exception guarantee
// so it stays unchanged is case of thrown an exception
// It uses separated object for memory management, that could be used with private inheritance
// or aggregation
// Requirements to T: default constructor (much less than first version)


// implementation changed from C++99, that is Satter described
#if 0
// Class for the memory management
template <typename T>
class StackImpl{
protected:

    // constructor is exception-safe
    // param initial_capacity: pre-allocated memory capacity
    StackImpl(size_t initial_capacity = 0) :
        _v(operator new(sizeof(T)*initial_capacity)),
        _vsize(),
        _vcapacity(initial_capacity){}

    // destructor is notrow
    ~StackImpl(){

        // call destructor for every T
        destroy(_v, _v + _vcapacity);

        // release memory
        operator delete(_v);
    }

    // Swap is nothrow
    void Swap(const StackImpl& other){
        swap(_v, other._v);
        swap(_vsize, other._vsize);
        swap(_vcapacity, other._vcapacity);
    }

    // memory buffer
    T* _v;

    // Stack size
    size_t _vsize;

    // Allocated memory size
    size_t _vcapacity;

private:
    StackImpl(const StackImpl&);
    StackImpl& operator=(const StackImpl&);
};


// Stack implementation
template <typename T>
class Stack2 : private StackImpl<T>{
public:

    // Constructor is exception-safe
    Stack2(size_t initial_capacity = 0) : StackImpl<T>(initial_capacity){}

    // We don't need destructor!

    // In case of exception StackImpl correctly release all memry
    Stack2(const Stack2& other) : StackImpl<T>(other._vsize){
        // We do not call StackImpl constructor here
        while (_vsize < other._vsize) {
            construct(_v + _vsize, other._v[_vsize]);
            ++_vsize;
        }
    }

    // No-throw guarantee
    Stack2& operator=(const Stack2& other){
        Stack2 tmp(other);
        Swap(tmp);
        return *this;
    }

    // Does not throw
    size_t Count() const {
        return _vsize;
    }

    // Exceptions may occur only with tmp object
    // We do not affect our while we don't finish
    void Push(const T& t){

        if (_vsize == _vcapacity){

            Stack2 tmp(2 * _vcapacity + 1);
            while (tmp.Count() < _vsize) {
                tmp.Push(_v[tmp.Count()]);
            }
            tmp.Push(t);
            Swap(tmp);
        }
        else{
            std::allocator<T> a;
            a.construct<T>(_v + _vsize, t);
            ++_vsize;
        }
    }

    T& Top(){  // throws if empty 

        if (_vsize == 0){
            throw std::runtime_error("Stack is empty");
        }
        return _v[_vsize - 1];
    }

    void Pop(){ // throws if empty

        if (_vsize == 0){
            throw std::runtime_error("Stack is empty");
        }
        else{
            --_vsize;
            std::allocator<T> a;
            a.destroy(_v + _vsize);
        }

    }
};


/////////////////////////////////////////////
void show_safe_stack2(){
    Stack2<int> ss;
    ss.Push(1);
    ss.Push(2);
    size_t sz = ss.Count();
    // Unsafe!
    // int i = ss.pop();
    int i = ss.Top();
    ss.Pop();
}
#endif

// uncaught_exception() reports that some exception is thrown and not processed yet

int main(){
    show_safe_stack1();
    return 0;
}
