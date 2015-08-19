#include <iostream>
#include <numeric>


// don't use in this example
//using namespace std;

// Task 5.3-4 Satter 1

// Satter 5.3
// What to prefer while overloading binary operators - friend operator or operator that access open interface

namespace output_friend{
    class X{
        friend std::ostream& operator<<(std::ostream& o, const X& x);
    };
    std::ostream& operator<<(std::ostream& o, const X& x){ return o; }
}

namespace output_nonfriend{
    class X{
    public:
        std::ostream& print(std::ostream& o) const{ return o; }
    };
    std::ostream& operator<<(std::ostream& o, const X& x){
        return x.print(o);
    }
}

// Correct answer - no difference. In both cases opeator is the interface part (according to Satter 5.2)
// Interface could be "compound" as the namespaces could be merged

// Satter 5.4. Complicated example

// Hide names and Kenig lookup
// Example
struct B
{
    void f(int){}
    void f(double){}
    void g(int){}
};

struct D : public B
{
private:
    void f(int, bool){}
};

void show_hide_names(){
    D d;
    int i = 1;
    
    // error C2660: 'D::f' : function does not take 1 arguments
    //d.f(i);
    // D::f hides B::f
    // How f() lookup happen?
    // 1. It starts in D (and success)
    // 2. ONLY if it is not successful, it is being looked for in base class, wrapping namespace, global namespace.
    // Compiler works with overloads that were found only in ONE scope (D in our case)

    d.g(i);
}

// The following code does not compile:
#if 0

namespace N{ class C{}; }

// This operator+ will not be found from std::accumulate!
int operator+(int i, N::C){ return i + 1; }

#else
namespace N{ 
    class C{}; 
    int operator+(int i, N::C){ return i + 1; }
}

#endif

void show_compliceted(){
    N::C a[10] = {};

    // start looking for operator+ in std namespace, and find it in <numeric> header!
    // It is template, and does not work! :(
    std::accumulate(a, a + 10, 0);

    // So it will never consider operator+ from global namespace
}

int main(){
    show_hide_names();
    show_compliceted();
    return 0;
}
