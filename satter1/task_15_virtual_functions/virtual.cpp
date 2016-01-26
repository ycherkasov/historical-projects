#include <complex>
#include <iostream>

using namespace std;

// Task 3.2, 3.10, 3.11 Satter 1
// 3.2 Classic example with polymorphisms
// 3.10 All cases of pure-virtual function implementation (create body)
// 3.11 Allow class polymorphisms only in certain places (functions, classes)

// 3.2 Classic example

class base{
public:
    virtual void f(int){
        cout << "base::f(int)\n";
    }
    virtual void f(double){
        cout << "base::f(double)\n";
    }
    virtual void g(int i = 10){
        cout << "base::g(int) g = " << i << '\n';
    }
};

class derived : public base{
public:

    virtual void f(complex<double>){
        cout << "derived::f(complex<double>)\n";
    }

    virtual void g(int i = 20){
        cout << "derived::g(int) g = " << i << '\n';
    }

};

void show_virtual(){

    base b;
    derived d;
    base* pb = new derived;

    // call base::f(double), no surprise
    b.f(1.0);

    // derived class do not overload f(), it HIDES it
    d.f(1.0);
    
    // called base::f() - overload has static resolve
    // C++ does see f() function, but it's not associated with base::f virtualness 
    // due to function signature difference
    pb->f(1.0);

    // 10, ok
    b.g();

    // 20, direct g() call
    d.g();

    // 10 - default params has static resolve
    pb->g();

    // unsafe - there's no virtual destructor
    delete pb;
}

/*
Satter 3.10
All cases of pure-virtual function implementation:
1. Pure virtual destructor
2. Implementation by default
3. Common partial implementation (protected)
4. Throw an exception if pure virtual call is possible
*/

// Satter 3.11

// The task:

// Some hierarchy...
class Base{
public:
    virtual void VirtFunc() const {
        cout << "Base::VirtFunc\n";
    }
};

class Derived : public Base{
public:
    virtual void VirtFunc() const {
        cout << "Derived::VirtFunc\n";
    }
};

// ...its user
void SomeFunc(const Base& b){
    b.VirtFunc();
}

// ... and high-level users

void func1(){
    // We allow usage here:
    Derived d;
    SomeFunc(d);
}

void func2(){
    // We want to deny usage here:
    Derived d;
    SomeFunc(d);
}

// The solution:
class Base2{
public:
    virtual void VirtFunc() const {
        cout << "Base::VirtFunc\n";
    }
};

// use private instead
class Derived2 : private Base2{
public:
    virtual void VirtFunc() const {
        cout << "Derived::VirtFunc\n";
    }
    // open for  one function only!
    friend void func3();
};

// ...its user
void SomeFunc2(const Base2& b){
    b.VirtFunc();
}



void func3(){
    // We allow usage here:
    Derived2 d;
    SomeFunc2(d);
}

void func4(){
    // We want to deny usage here:
    Derived2 d;
    // Won't be compiled!
    //SomeFunc2(d);
}


void show_switching_polymorph(){
    func1();
    func2();
    func3();
    func4();
}

int main(){

    show_virtual();
    show_switching_polymorph();
    return 0;
}
