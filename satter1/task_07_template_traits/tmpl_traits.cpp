#include <cassert>
#include <iostream>

using namespace std;

// Task 1.11 From Satter 1
// Templates inheritance and traits


//////////////////////////////////////////////////////////////////////////
// 1. Require for member function exists, member pointer way

template <typename T>
class RequiresClone{
public:

    // WARNING: this function could not be instantiated
    // In this case we won't get a compilation error even if T does not have Clone() member
    void func(const T* t){
        T* other = t->Clone();
        delete(other);
    }

    // SOLVE: ensure that T has clone by creation pointer to this member
    // and do it in the function that be instatntitted in any case
    // and exists always, e.g. destructor.
    // That's better to wrap it with function
    inline bool hasClone(){
        T* (T::*test_clone)() const = &T::Clone;
        return test_clone ? true : false;
    }

    ~RequiresClone(){
        assert(hasClone());
    }
};

// Classes for test
struct HasClone {
    HasClone* Clone() const {
        return new HasClone();
    }
};

struct DontHasClone {};

void check_has_method1(){
    HasClone h;
    RequiresClone<HasClone> r1;
    r1.func(&h);

    DontHasClone d;
    d;
    // Could not instatntiate class!
    //RequiresClone<DontHasClone> r2;
}

// The same method could be reused for any class using inheritance
template <typename T>
class IsHasClone{
    // MUST have the only constructor
public:
    IsHasClone(){
        assert(Constraint());
    }
private:
    static bool Constraint(){
        T* (T::*test_clone)() const = &T::Clone;
        return test_clone ? true : false;
    }
};

// Use of class above
template <typename T>
struct RequiresClone2 : IsHasClone<T> {
    HasClone* func(const T* t) const {
        return t->Clone();
    }
};

void check_has_method2(){
    HasClone h1;
    RequiresClone2<HasClone> r;
    HasClone* h2 = r.func(&h1);
    delete(h2);
}

// Cloneable Class
template <typename T>
struct Cloneable{
    T* Clone(T* t){
        return new T(*t);
    }
};

// Add Clone() method to any class
class ShouldBeCloned : public Cloneable<ShouldBeCloned>{
    int i;
public:
    ShouldBeCloned(){}
};

void check_has_method3(){
    ShouldBeCloned cloned;
    ShouldBeCloned* cl1 = cloned.Clone(&cloned);
    delete(cl1);

}



//////////////////////////////////////////////////////////////////////////
// 2. Require for inheritance, SFINAE
template <typename Child, typename Parent>
class IsDerivedFrom {
private:
    struct No{};
    struct Yes{char c[2];};
    static Yes Test(Parent*){
        Yes y; return y;
    }

    static No Test(...){
        No n; return n;
    }

public:
    enum {IsDerived = sizeof( Test( static_cast<Child*>(0) ) ) == sizeof(Yes) };
};

class A{
public:
    int i;
};

class B : public A{
public:
    double d;
};


void check_is_derived1(){
    typedef IsDerivedFrom<B, A> CheckDerived;
    bool b = CheckDerived::IsDerived;
    cout << "B derived from A = " << b << endl;

    typedef IsDerivedFrom<A, B> CheckDerived2;
    bool b2 = CheckDerived2::IsDerived;
    cout << "A derived from B = " << b2 << endl;
};

//////////////////////////////////////////////////////////////////////////
// 2. Require for inheritance, check pointer to parent and child
template <typename Child, typename Parent>
class IsDerivedFrom2 {
private:
    static void Constraint(Child* ch){
        // could not assign pointer to non-derived class!
        Parent* p = ch;
        p = ch;
    }
public:
    IsDerivedFrom2(){
        void(*p)(Child*) = Constraint;
        assert(p);
    }
};

void check_is_derived2(){
    typedef IsDerivedFrom2<B, A> CheckDerived;
    CheckDerived check1;
    typedef IsDerivedFrom2<A, B> CheckDerived2;
    // COMPILE-TIME CHECK! could not silently convert Child* to Parent*
    //CheckDerived2 check2;
};

//////////////////////////////////////////////////////////////////////////
// 3. Require for inheritance, combine SFINAE and pointes casts
template <typename Child, typename Parent>
class IsDerivedFrom3 {
private:
    struct No{};
    struct Yes{char c[2];};
    static Yes Test(Parent*){
        Yes y; return y;
    }

    static No Test(...){
        No n; return n;
    }

    static void Constraint(Child* ch){
        // could not assign pointer to non-derived class!
        Parent* p = ch;
        p = ch;
    }

public:
    IsDerivedFrom3(){
        void(*p)(Child*) = Constraint;
        assert(p);
    }

    enum {IsDerived = sizeof( Test( static_cast<Child*>(0) ) ) == sizeof(Yes) };
};

//////////////////////////////////////////////////////////////////////////
// 3. Extend for inheritance, if the class does not contain Cloneable() method

// Cloneable interface and classes for testing
template <typename T>
struct CloneableInterface{
    T* Clone(T* t){
        return new T(*t);
    }
};


struct C : public CloneableInterface<C> {
    int i;
    C():i(){}
};


struct D {
    int i;
    D():i(){}
};

// Common implementation
template <typename T, int>
struct CloanebleSwitcher{
    // T is not derived from Cloneable
    // add owr own Clone()
    T* _clone(T* clone_me){
        cout << "T is not derived from Cloneable" << endl;
        cout << "Use common clone code" << endl;
        return new T(*clone_me);
    }
};

// Specialized implementation
template <typename T>
struct CloanebleSwitcher<T, 1>{
    // T is derived from Cloneable
    // already has Clone(), use it
    T* _clone(T* clone_me){
        cout << "T is derived from Cloneable" << endl;
        cout << "Use base class method" << endl;
        // pass pointer to itself not to downcast in CLone()
        return clone_me->Clone(clone_me);
    }
};

// Clone() implementation switcher
template <typename T>
class HasCloneeDepends{
    T* _clone_me;
public:
    HasCloneeDepends(T* t): _clone_me(t){}

    CloanebleSwitcher<T, IsDerivedFrom<T, CloneableInterface<T> >::IsDerived  > _impl;

    T* Clone(){
        return _impl._clone(_clone_me);
    }
};


void check_and_inherit(){
    C base;
    HasCloneeDepends<C> h1(&base);
    C* c = h1.Clone();

    D independent;
    HasCloneeDepends<D> h2(&independent);
    D* d = h2.Clone();
    delete(c);
    delete(d);
}

int main(){
    check_has_method1();
    check_has_method2();
    check_has_method3();
    check_is_derived1();
    check_is_derived2();
    check_and_inherit();
    return 0;
}

