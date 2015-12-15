#include <iostream>

/*
Chapter 06 Jeff Eldger

* smart pointers that really owns their objects (master pointers)
* descriptior-pointers that save a reference to master pointer
* read-only pointers make all own memory constant
*/

//////////////////////////////////////////////////////////////////////////
// traits class that could describe cloning semantic (also delete and other)
template <typename T>
struct value_pointer_traits{
    static T* clone(const T* p) {
        return new T(*p);
    }
};


// smart pointer that really owns their objects (master pointer)
template <typename T>
class value_ptr{
public:

    // construction and destruction - always create by default
    // variable number of params could be created with variadics
    explicit value_ptr() : _p(new T){}

    ~value_ptr(){
        delete _p;
    }

    // copy and assignment
    value_ptr(const value_ptr& other) : _p(create_from(other._p)){}

    value_ptr& operator=(const value_ptr& other){
        value_ptr temp(other);
        swap(temp);
        return *this;
    }

    void swap(value_ptr& other){
        std::swap(_p, other._p);
    }

    // template copy and assignment (do not replace own copy and assignment)

    template <typename U>
    value_ptr(const value_ptr<U>& other) : _p(create_from(other._p)){}

    template <typename U>
    value_ptr& operator=(const value_ptr<U>& other) {
        value_ptr temp(other);
        swap(temp);
        return *this;
    }

    // pointer semantic (const and non-const)
    const T& operator*() const {
        return *_p;
    }

    const T* operator->() const {
        return _p;
    }

    T& operator*() {
        return *_p;
    }

    T* operator->() {
        return _p;
    }

protected:

    // all classes we copy from are our friends!
    template <typename U> friend class value_ptr;

    // copy from other type value_ptr
    template <typename U>
    T* create_from(const U* p) const {
        return p ? static_cast<T*>(value_pointer_traits<U>::clone(p)) : new T;
    }

private:
    T* _p;
};

struct A{
    int i;
    A() :i(){}
    virtual ~A(){}
};

struct B : public A{
    int i;
    B() :i(){}
};

void show_master_ptr(){
    value_ptr<A> a1; // new A
    value_ptr<A> a2(a1);
    value_ptr<A> a3;
    a3 = a1;

    value_ptr<B> b1;
    value_ptr<B> b2(a1);
    b1 = a1;
}

//////////////////////////////////////////////////////////////////////////
// read-only pointers (if we want to make an object constant)
template <typename T>
class read_only_ptr{
public:

    // create-destroy
    read_only_ptr() : ptr_(new T){}
    read_only_ptr(const T* p) : ptr_(p){}

    ~read_only_ptr(){
        delete ptr_;
    }

    // only create from other
    read_only_ptr(const read_only_ptr& other) : ptr_(new T(*other.ptr_)){}

    // no assign
    read_only_ptr& operator=(const read_only_ptr& other) = delete;

    const T* operator->() {
        return ptr_;
    }

private:
    const T* ptr_;
};

void shwo_readonly_ptr(){
    read_only_ptr<A> a1;
    read_only_ptr<A> a2(new A);
    read_only_ptr<B> b1;

    // error C3892: 'a1' : you cannot assign to a variable that is const
    // a1->i = 5;
}

int main(){
    show_master_ptr();
    shwo_readonly_ptr();
    return 0;
}
