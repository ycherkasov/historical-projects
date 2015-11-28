#include <iostream>
#include <algorithm>
#include <list>

using namespace std;

// Task 3.5 Satter 1
// Inheritance vs Aggregation

// Here's the base template container
template <typename T>
class MyList{
public:

    void Instert(const T& t){
        _l.push_back(t);
    }

    T& Access(size_t index){
        // no check
        auto it = advance(_l.begin(), index);
        return *it;
    }

    void Size() const {
        return _l.size();
    }

private:
    list<T> _l;
};

//... and two use cases

// 1. Private inheritance
template <typename T>
class MySetInherit : private MyList<T>{
public:

    // Reuse base class method
    void Add(const T& t){
        MyList<T>::Instert(t);
    }

    // Reuse base class method
    T& Get(size_t s){
        return MyList<T>::Access(s);
    }

    // In case of inheritance we can open base class methods
    using MyList<T>::Size;

private:
};

// 2. Aggregation
template <typename T>
class MySetIncl {
public:
    // Reuse base class method
    void Add(const T& t){
        _list_impl.Instert(t);
    }

    // Reuse base class method
    T& Get(size_t s){
        _list_impl.Access(s);
    }

    // In case of aggregation we must directly call public methods
    size_t Size() const {
        return _list_impl.Size();
    }

private:
    MyList<T> _list_impl;
};

// Aggregation is less coupling than inheritance
// It could be used in several cases:
// - replace virtual function
// - access to protected member
// - create before some base or aggregated class
// - mutual usage of some more base class or interface

// In all other cases aggregation is better

int main(){

    MySetInherit<int> set0;
    MySetIncl<int> set1;

    set0.Add(1);
    set1.Add(1);

    return 0;
}
