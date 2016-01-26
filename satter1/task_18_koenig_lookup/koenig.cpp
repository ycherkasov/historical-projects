#include <iostream>
#include <string>

// don't use in this example
//using namespace std;

// Task 5.1-2 Satter 1
// Kenig Lookup examples

namespace A{

    struct X{};
    struct Y{};
    void f(int){ std::cout << "A::f(int)\n"; }
    void g(X){ std::cout << "A::g(X)\n"; }
}

namespace B{

    void f(int i){
        // warning C4717 : 'B::f' : recursive on all control paths, function will cause runtime stack overflow
        // We probably wanted to call A::f, but we didn't opened A namespace elsewhere 
        std::cout << "B::f(int)\n";
        f(i);
    }

    void g(A::X x){
        std::cout << "B::g(A::X x)\n";
        //g(x);// - compilation error, Kenig lookup opens A namespace because of function argument
    }

    void h(A::Y y){
        std::cout << "B::g(A::Y y)\n";
        // warning C4717: 'B::h' : recursive on all control paths, function will cause runtime stack overflow
        // endless recurion as expected
        h(y);
    }
}

// More common Kenig lookup example
namespace NS{
    class T{};
    void f(T t){}
}

NS::T param;


// Satter1 5.2 Class interface - not only methods, but also functions that use this type
// So
class X{
    // interface

    // could be here:
    // void use_x(); - X& could be considered as 'this'
};

// also interface, ONLY is supplied with X
// e.g. in other files or libraries X users are just clients
void use_x(const X& x);

// Good example are operators (+-*/<<>>)
// That's why Kenig lookup is necessary!

void show_kenig_lookup(){
    using namespace B;
    A::X x;
    A::Y y;
    //f(1); - recursive call B::f - we don't see namespace A
    //g(x); - does not compile, x opens A namespace
    // h(y); - recursive call B::h

    f(param); // example from Standard, NS namespace is opened in show_kenig_lookup() due to function param NS::T

    // The same is how std::ostream works
    // one more example
    std::string s = "Kenig loookup";
    std::cout << s << std::endl; // operator << is found using Kenig lookup, we don't use using namespace std elsewhere
}
// So namespaces not so independent
// This is because functions that operate with the class ptr/ref are also part of its interface

// Meyers example - works fine, until we add f() to MayersA namespace

namespace MeyersA{
    struct X{};

    // Could not compile: ambiguous call to overloaded function
    // void f(X x){}

    // This is correct, as f() is a part of MeyersA::X interface,
    // even it is defined in MeyersB
}

namespace MeyersB{
    
    void f(MeyersA::X x){}

    void g(MeyersA::X x){
        f(x);
    }
}

void meyers_example(){
    using namespace MeyersB;
    MeyersA::X x;
    g(x);
}


int main(){
    show_kenig_lookup();
    meyers_example();
    return 0;
}
