#include <iostream>

using namespace std;


// check operator <> and implicit cast to plain pointer

// We want to implement smart pointer with correct deep copying semantic

// We might want the following additional features
// * template copying and assignment (for polymophic usage)
// * copying semantic traits

// traits class that could describe cloning semantic (also delete and other)
template <typename T>
struct value_pointer_traits{
    static T* clone(const T* p) {
        return new T(*p);
    }
};

template <typename T>
class value_ptr{
public:

    // construction and destruction - could be expanded by deleter traits
    explicit value_ptr(T* p = nullptr) : _p(p){}

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

    // implicit cast operator!
    // this is not recommended, just for example
    operator T*(){
        return _p;
    }

    // implicit cast operator!
    // this is not recommended, just for example
    operator T*() const {
        return _p;
    }

    // do not compare line plain pointers
    bool operator>(const value_ptr& other) = delete;
    bool operator<(const value_ptr& other) = delete;

protected:

    // all classes we copy from are our friends!
    template <typename U> friend class value_ptr;

    // copy from other type value_ptr
    template <typename U>
    T* create_from(const U* p) const {
        return p ? static_cast<T*>(value_pointer_traits<U>::clone(p)) : nullptr;
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

void show_value_ptr(){
    value_ptr<A> a1(new A);
    value_ptr<A> a2(a1);
    value_ptr<A> a3;
    a3 = a1;

    value_ptr<B> b1;
    value_ptr<B> b2(a1);
    b1 = a1;

    // test implicit cast
    A* pa = a1;
    B* pb = b2;

    // compare plain pointers works
    bool b = (pa > pb);

    // However we can't compare smart pointers even they are implicitly converted to plain!!!
    // as operator > explicitly deleted
    //b = (a1 > b2);
}

int main(){
    show_value_ptr();
    return 0;
}
