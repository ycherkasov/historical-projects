#include <iostream>
#include <string>

// Task 10.3 Satter 1
// const and mutable usage
namespace const_functions {
// Base type parameters passed by value don't affected by 'const' modifier

// These function prototypes are identical
// Don't use const with basic types
void f(int i){}
//int f(const int i){}

// This does not true for references
// These functions are different
void g(int& i){}
void g(const int& i){}

// Returning object by value do it const, so that not to change temporary object on stack
const std::string get_string(){ return "str"; }

struct A{
    int i = 0;
};

}

void show_const_casting(){

    using namespace const_functions;
    A a1;
    const A a2;

    // non-const object to non-const reference
    A& ra1 = a1;

    // This is UB, const_cast casts only const references or pointers, not const objects!
    // A& ra2 = const_cast<A&>(a2);

    // Setting const using const_cast does not make sense
    // const A& = const_cast<const A&>(a1);
    // ... it can be done just by assignment
    const A& ra3 = a1;

    // this is correct, const_cast removes constant only from references or pointers
    A& ra4 = const_cast<A&>(ra3);

    // volatile also could be removed
}

// Satter 1 10.4 Casting operations
namespace show_casting_{

class A{
public:
    virtual ~A(){}
};

class B : private virtual A{};

class C : public A{};

class D : public B, public C {};

}


void show_casting(){
    using namespace show_casting_;

    A a1;
    B b1;
    C c1;
    D d1;

    const A a2;
    const A& ra1 = a1;
    const A& ra2 = a2;
    char c;

    // C-style casting
    A* pa;
    pa = (A*)&ra1; // use const_cast instead
    pa = (A*)&a2;  // UB as a2 is a const object (see show_const_casting()) 
    B* pb = (B*)&c1;    // use reinterpret_cast instead (all other are compilation error)
    C* pc = (C*)&d1;    // C and D are connected by inheritance. dynamic_cast could be used to check. 
    
    // direct assignment also could be used
    pc = &d1;

    // This is incorrect. char/unsigned char/signed char are 3 different types
    // static_cast does not cast them
    //unsigned char* puc = static_cast<unsigned char*>(&c);
    //signed char* psc = static_cast<signed char*>(&c);

    // This is correct
    unsigned char* puc = reinterpret_cast<unsigned char*>(&c);
    signed char* psc = reinterpret_cast<signed char*>(&c);

    // Everything is correct
    void* pv = static_cast<void*>(&b1);

    // Moreover, any pointer could be casted to void* implicitly
    pv = &b1;

    // Correct, static_cast from void
    B* pb1 = static_cast<B*>(pv);

    // Useless, as the argument anyway is B*
    B* pb2 = static_cast<B*>(&b1);

    // Correct
    A* pa1 = const_cast<A*>(&ra1);

    // UB as ra2 -> a2, that is const
    A* pa2 = const_cast<A*>(&ra2);

    // Correct, but returns 0
    B* pb3 = dynamic_cast<B*>(&c1);

    // Error: inheritance is private, B not A 
    // A* pa3 = dynamic_cast<A*>(&b1);

    // Correct. Could be done without direct casting
    B* pb4 = static_cast<B*>(&d1);
    pb4 = &d1;

    // Correct, base-to-derived could be static_cast (but it is without runtime check)
    D* pd1 = static_cast<D*>(pb4);

    // Incorrect due to private inheritance (pb2 -> b1)
    pa1 = dynamic_cast<A*>(pb2);

    // Correct due to Satter, as inheritance A-B is private, but pb4 -> d1 (that is A)
    // However, anyway returns 0.
    pa1 = dynamic_cast<A*>(pb4);

    // Correct, pb4 -> d1, could be casted to C*
    C* pc1 = dynamic_cast<C*>(pb4);

    // bad_cast exception here, pb2 -> B, B is not connected to C
    // C& rc1 = dynamic_cast<C&>(*pb2);
}

// The only good usage of setting const using const_cast - calling const/non-const overloaded template
// Very rare case, but...
template <typename T>
void f1(T&){}

template <typename T>
void f1(const T&){}

template <typename T>
void g(T& t){
    f1(t);
    f1(const_cast<const T&>(t));
}


// Satter1 10.5
// Could we emulate bool type
// Answer is NO, because:
// * emulate using typedef/#define we can't overload
// * can't use implicit casting

int main(){

    show_const_casting();

    show_casting();

    return 0;
}
