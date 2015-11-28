#include <iostream>
#include <string>

using namespace std;

// Task 3.7-3.9 Satter 1
// Multiple inheritance
// When it needed
// How to emulate


// When it could be needed:
// 1. Combine (proprietary) library interfaces, fasade pattern
// 2. Interface classes
// 3. Switch interfaces like COM


// Satter1 3.8 How to emulate

// Base classes for experiment
class A
{
public:

    virtual ~A(){}

    string Name(){
        return DoName();
    }

private:
    virtual string DoName(){
        return "A";
    }
};

class B1 : virtual public A {
    virtual string DoName(){
        return "B1";
    }
};

class B2 : virtual public A {
    virtual string DoName(){
        return "B2";
    }
};

//#define MULTIPLE_INHERITANCE

#ifdef MULTIPLE_INHERITANCE
// Here's multiple inheritance example
class D : public B1, public B2{
    virtual string DoName(){
        return "D";
    }
};

#else

// Here's its emulation example
// Very complicated and enormous
class D : public B1{
public:

    class D2 : public B2{
    public:
        void Set(D* d){ _d = d; }
    private:
        string DoName(){ return _d->DoName(); }
        D* _d;
    };

    // Constructor with "internal proxy" set
    D(){
        d2.Set(this);
    }

    D(const D& other) : B1(other), d2(other.d2){
        d2.Set(this);
    }

    D& operator=(const D& other){
        B1::operator=(other);
        d2 = other.d2;
        return *this;
    }

    // can cast to reference, but unable to pointer
    operator B2&(){
        return d2;
    }

    // Use separate method so that dereference:
    // B2* pb = &d.AsB2()
    // dynamic_cast to B2* won't be work
    B2& AsB2(){
        return d2;
    }

private:
    D2 d2;
    string DoName(){ return "D"; }
};

#endif

// Functions that use these classes
void f1(A& x) { cout << "f1: " << x.Name() << endl; }
void f2(B1& x){ cout << "f2: " << x.Name() << endl; }
void f3(B2& x){ cout << "f3: " << x.Name() << endl; }

void g1(A x) { cout << "g1: " << x.Name() << endl; }
void g2(B1 x){ cout << "g2: " << x.Name() << endl; }
void g3(B2 x){ cout << "g3: " << x.Name() << endl; }

void use_multiple_inheritance(){

    // Result class that uses multiple inheritance
    D d;

    // Cast by pointer
    B1* pb1 = &d;
#ifdef MULTIPLE_INHERITANCE
    B2* pb2 = &d;
#else
    B2* pb2 = &d.AsB2();
#endif

    // Cast by reference
    B1& rb1 = d;
    B2& rb2 = d;

    // Polymorphisms
    f1(d);
    f2(d);
    f3(d);

    // Slicing
    g1(d);
    g2(d);
    g3(d);

    // check dynamic_cast/RTTI
    // (in case of emulation does not work)
    cout << ((nullptr != dynamic_cast<D*>(pb1)) ? "ok\n" : "bad\n");
    cout << ((nullptr != dynamic_cast<D*>(pb2)) ? "ok\n" : "bad\n");

    try{
        D& d1 = dynamic_cast<D&>(rb1);
        cout << "ok\n";
        D& d2 = dynamic_cast<D&>(rb2);
        cout << "ok\n";
    }
    catch (...){
        cout << "bad\n";
    }
}

// Satter1 3.9 "SiamTwin" decoupling

class BaseA
{
public:
    virtual int ReadBuf(const char*){
        cout << "BaseA::ReadBuf\n";
        return 0;
    }
};

class BaseB
{
public:
    virtual int ReadBuf(const char*){
        cout << "BaseB::ReadBuf\n";
        return 0;
    }
};

class Derived : public BaseA, public BaseB{
public:
    // Hides both implementations
    virtual int ReadBuf(const char*){
        cout << "Derived::ReadBuf\n";
        return 0;
    }
};

// Solution:
class BaseA2 : public BaseA
{
public:
    // to keep it virtual is Derived
    virtual int ReadBufA(const char*) = 0;
private:
    int ReadBuf(const char* p){
        return ReadBufA(p);
    }
};

class BaseB2 : public BaseB
{
public:
    // to keep it virtual is Derived
    virtual int ReadBufB(const char*) = 0;
private:
    int ReadBuf(const char* p){
        return ReadBufB(p);
    }
};

class Derived2 : public BaseA2, public BaseB2{
public:
    // Open implementations
    virtual int ReadBufA(const char* p){
        cout << "Derived2::ReadBufA\n";
        BaseA::ReadBuf(p);
        return 0;
    }

    virtual int ReadBufB(const char* p){
        cout << "Derived2::ReadBufB\n";
        BaseB::ReadBuf(p);
        return 0;
    }
};


void show_siamtwin_decoupling(){
    
    // This is interface with SiamTwin problem
    Derived d;
    BaseA* pa = &d;
    BaseB* pb = &d;

    // Derived::ReadBuf() is called in every case
    d.ReadBuf("atatat");
    pa->ReadBuf("atatat");
    pb->ReadBuf("atatat");

    // Solution:
    Derived2 d2;
    BaseA* pa1 = &d2;
    BaseB* pb1 = &d2;

    // Correct methods called by pointer
    d2.ReadBufA("atatat");
    d2.ReadBufB("atatat");
    pa1->ReadBuf("atatat");
    pb1->ReadBuf("atatat");

}

int main(){

    use_multiple_inheritance();

    show_siamtwin_decoupling();

    return 0;
}
