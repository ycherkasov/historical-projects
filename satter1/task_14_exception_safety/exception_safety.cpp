#include <cassert>
#include <iostream>
#include <memory>

using namespace std;

// Task 2.16-2.19 From Satter 1
// Problems of exception safety and expressions validation order


// 2.16-17 Raw pointers problems

struct A{
    void do_work(){
        cout << "A::do()\n";
    }
};

struct B{
    void do_work(){
        cout << "B::do()\n";
    }
};

// 1. Pass raw pointers
template <typename T1, typename T2>
void f1(T1* t1, T2* t2){
    t1->do_work();
    t2->do_work();
    delete t1;
    delete t2;
}

// 2. Pass smart pointers
// auto_ptr in Satter book example
template <typename T1, typename T2>
void f2(shared_ptr<T1> t1, shared_ptr<T2> t2){
    t1->do_work();
    t2->do_work();
}


// 2.18 Exception-safe classes

// This class is unable to make exception-safe
template <typename T1, typename T2>
class Widget{
public:
    // ...
    Widget& operator=(const Widget& other){

        // This operator is always exception-unsafe!
        // Because may leave class in inconsistent state
        T1 tmp1(other.t1);
        T2 tmp2(other.t2);
        swap(t1, tmp1);
        swap(t2, tmp2);
        return *this;
    }
private:
    T1 t1;
    T2 t2;
};

// The solution is...
template <typename WidgetImpl>
class Widget2{
public:
    // ...
    Widget2& operator=(const Widget2& other){

        // This operator is exception-safe
        WidgetImpl tmp(other.impl);
        swap(impl, tmp);
        return *this;
    }
private:
    WidgetImpl impl;
};

// 2.18 Exception-safe classes - inheritance or aggregation?
// Replace in previous example aggregation with the private inheritance
template <typename WidgetImpl>
class Widget3 : private WidgetImpl{
public:
    // ...
    Widget3& operator=(const Widget3& other){

        // Implementation operator= could be exception-unsafe!
        WidgetImpl::operator=(other);
        
        // may be doing something else
        return *this;
    }
};


void usage_raw_pointers(){

    // unsafe because order is undefined and one of objects could cause memory leak
    f1<A,B>(new A, new B);

    // the same problem. Memory leak could happen before calling shared_ptr constructor
    // (really? check actual Standard)
    f2<A,B>( shared_ptr<A>(new A), shared_ptr<B>(new B));

    // The most correct usage
    shared_ptr<A> a(new A);
    shared_ptr<B> b(new B);
    f2(a, b);

}

void usage_exception_safe(){
    Widget<A, B> w1;
    Widget<A, B> w2;
    w2 = w1;
}

int main(){

    // show problems with pointers, exceptions and valuation order
    usage_raw_pointers();

    // Exception safe and unsafe classes
    usage_exception_safe();

    return 0;
}
