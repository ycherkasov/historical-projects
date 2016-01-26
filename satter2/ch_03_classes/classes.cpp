#include <iostream>
#include <complex>

// From Satter 2. Ch.03 Classes

// Ch 3 Task 14 Class construction order
namespace class_construction_order{

class B1{
public:
    B1(){ std::cout << "B1\n"; }
};
class V1 : public B1{
public:
    V1(){ std::cout << "V1\n"; }
};
class D1 : virtual public V1{
public:
    D1(){ std::cout << "D1\n"; }
};

class B2{
public:
    B2(){ std::cout << "B2\n"; }
};
class B3{
public:
    B3(){ std::cout << "B3\n"; }
};
class V2 : public B1, public B2{
public:
    V2(){ std::cout << "V2\n"; }
};
class D2 : public B3, virtual public V2{
public:
    D2(){ std::cout << "D2\n"; }
};

class M1{
public:
    M1(){ std::cout << "M1\n"; }
};
class M2{
public:
    M2(){ std::cout << "M2\n"; }
};

class X : public D1, public D2{
public:
    X(){ std::cout << "X\n"; }

    M1 m1;
    M2 m2;
};

/*
Hierarchy

B1  B1 B2
|   |__|
V1   V2  B3
|    |v _|
D1    D2
|v_ __|
   X

1 Step: Virtual base classes
V1: B1, V1
V2: B1, B2, V2
2 Step. Non-virtual base classes
D1: D1
D2: B3, D2
3 Step. Members
M1, M2
4 Step. X itself
*/

}

void show_construction_order(){
    using namespace class_construction_order;
    X x;
}


// Ch 3 Task 15 Hacking the class
namespace hacking_the_class{

// Q: is there any 'legal' ways to get access to int steal_me_ without changing the class?
class X{
public:

    X() :steal_me_(1){}

    int value(){ return steal_me_; }

    template <typename T>
    void f(const T& t){}

private:
    int steal_me_;
};

//A: 
//1. The most dirty hack, if we have access to codebase
//#define private public

//2. More sophisticated, if we know the data member order
struct thief{
    int i_steal_you;
};

void do_steal(X& x){
    reinterpret_cast<thief&>(x).i_steal_you = 2;
}

// 3. This way is absolutely legal, even if we don't know the exact placement of data member
struct Y{};

template <>
void X::f(const Y&){
    steal_me_ = 3;
}

}

void show_steal_private(){
    using namespace hacking_the_class;
    X x;
    std::cout << "value = " << x.value() << std::endl;

    do_steal(x);
    std::cout << "value = " << x.value() << std::endl;

    Y y;
    x.f(y);
    std::cout << "value = " << x.value() << std::endl;
}

// Ch 3 Task 16 Private members: accessibility and visibility
namespace private_visibility{

class calc_me{
public:
    typedef int(calc_me::* twice_pt)(int);

    double twice(double d){ return d * 2; }

    twice_pt get_private_method(){ return &calc_me::twice; }

// - Private members are inaccessible, but they are visible
// - Private members are inaccessible BY NAMES. It there any way to
// change the name using indirect access, it could be accessible
private:

    // visible, but inaccessible
    int twice(int d){ return d * 2; }

    // if we remove #include <complex> we get a compilation error even if the following method is private
    std::complex<double> twice(const std::complex<double>& d){ return d * std::complex<double>(2); }
};


}

void show_private_visibility(){
    using namespace private_visibility;
    calc_me c;
    
    // error C2248: 'private_visibility::calc_me::twice' : 
    // cannot access private member declared in class 'private_visibility::calc_me'
    // Because it if overloaded BEFORE checking access
    // c.twice(21);

    // It was done so that not to change behavior by changing accessibility

    // can not even get pointer to closed member by name 
    // error C2248 : 'private_visibility::calc_me::twice' : 
    // cannot access private member declared in class 'private_visibility::calc_me'
    // calc_me::twice_pt p = &calc_me::twice;

    // but we can using Public Morozov pattern
    calc_me::twice_pt p = c.get_private_method();
    int i = (c.*p)(2);
}

// Ch 3 Task 17 Encapsulation
// Encapsulation is not always hiding of data members, but hiding of data members is a special case of encapsulation
// In general, encapsulation is separating implementation and interface

// Arguments for closing data:

// - this is against encapsulation 
// - we expect that open data is our class member
// - change data member with some calculation or other class
// - logging, debugging


// Ch 3 Task 18 Virtual functions
// Highly recommended Non-Virtual Interface Pattern
// (used in STL and in .NET implementation)
// It's main point is in making leaf classes interface completely non-virtual
// All virtual functions are hidden as private or protected

// In case of virtual interface set of virtual function
// in the same time represent interface and polymorph behavior
// So, virtual interface represent BOTH interface and implementation
namespace virtual_interface{

class widget{
public:
    virtual int process(){ return 0; }
    virtual bool is_done(){ return true; }
};

}

// Non-virtual interface if more stable
// 1. Base class controls own interface
// 2. Interface and implementation are separated
// 3. Base classes could be changed without changing interface
namespace nvi{

class widget{
public:
    int process(){ return do_process1(); }
    bool is_done(){ return do_is_done(); }
private:
    virtual int do_process1(){ return 1; }
    virtual int do_process2(){ return 2; }
    virtual bool do_is_done(){ return true; }

};

}

// More NVI-like example: Bridge pattern (GOF), Handle-body pattern (Coplien)
// Base class destructor could be open and virtual or protected non-virtual
namespace destructor_nvi{

class BaseVirtual{
public:
    virtual ~BaseVirtual(){
        std::cout << "BaseVirtual\n";
    }
};

class DerivedVirtual : public BaseVirtual{
public:
};

class BaseNonVirtual{
protected:
    // deny polymorph usage
    ~BaseNonVirtual(){
        std::cout << "BaseNonVirtual\n";
    }
};

class DerivedNonVirtual : public BaseNonVirtual{
};

}

void show_nvi(){
    
    using namespace destructor_nvi;

    // TBD?
    virtual_interface::widget virt_w;
    nvi::widget nvi_w;

    BaseVirtual* bv = new DerivedVirtual;
    delete bv;

    //BaseNonVirtual* bnv = new DerivedNonVirtual;
    //delete bnv;
    // 'destructor_nvi::BaseNonVirtual::~BaseNonVirtual' : 
    // cannot access protected member declared in class 'destructor_nvi::BaseNonVirtual'

    // ok
    DerivedNonVirtual d;
}

// Ch 3 Task 19 Implicitly generated members
// * Implicitly generated members are defined only when called somewhere
// * Implicitly generated copy constructor and assignment operator has const A& signature
// ONLY if all members and base classes has such signature (contra-example is auto_ptr)
// non-const version is generated otherwise
// * operator= can be virtual and can be hidden in this case (don't make it virtual!)

int main(){

    show_construction_order();

    show_steal_private();

    show_private_visibility();

    show_nvi();

    return 0;
}
