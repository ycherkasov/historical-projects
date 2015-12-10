#include <iostream>
#include <stdexcept>
using namespace std;

/*
Chapter 05 Jeff Eldger

* smart pointers could have strategies on null derefefence (as Alexandrescu defined)
  - throw exception strategy
  - construct default strategy
  - construct from prototype (screamer) strategy
* smart pointers are good for cached objects (in-memory, handles, HDD etc)
*/

// Show some smart-pointers strategies when trying to dereference nullptr
struct test_me{
    void method(){
        cout << "test_me::method()" << endl;
    }
};

template <typename T>
struct exception_strategy{
    static T* on_null_pointer(){
        throw std::runtime_error("null pointer dereference");
        // never get there
        return nullptr;
    }
};

template <typename T>
struct default_value_strategy{
    static T* on_null_pointer(){
        return new T;
    }
};

template <typename T>
struct screamer_value_strategy{
    static T* on_null_pointer(){
        return new T(screamer);
    }
    // could be some default prototype with specialization
    static T screamer;
};

// static
template <typename T>
T screamer_value_strategy<T>::screamer;

template <typename T, typename NullPointerStrategy >
class simple_ptr{
public:

    // default constructor
    simple_ptr() :_ptr(){}

    // take ownership constructor
    explicit simple_ptr(T* mem) :_ptr(mem){}

    // release memory
    ~simple_ptr(){
        delete _ptr;
    }

    // Pointer semantic
    T* operator->() const {

        // use nullptr strategy
        if (nullptr == _ptr){
            _ptr = NullPointerStrategy::on_null_pointer();
        }
        return _ptr;
    };

    T& operator*() const {

        // use nullptr strategy
        if (nullptr == _ptr){
            NullPointerStrategy::on_null_pointer();
        }
        return _ptr;

        return *_ptr;
    };

    // Functional analog ->
    T* get() const {
        return _ptr;
    };

    // explicit release
    T* release() {
        T* ptr = _ptr;
        _ptr = 0;
        return ptr;
    };

private:
    // it could be pointer, struct, proxy, reader from HDD, other smart ptr...
    mutable T* _ptr;
};

void show_smart_pointers_strategies(){
    simple_ptr<test_me, exception_strategy<test_me>> p1;
    simple_ptr<test_me, default_value_strategy<test_me>> p2;
    simple_ptr<test_me, screamer_value_strategy<test_me>> p3;

    try{
        p1->method();
    }
    catch (const std::runtime_error& err){
        cout << err.what() << endl;
    }

    p2->method();

    p3->method();
}

int main(){
    show_smart_pointers_strategies();
    return 0;
}
