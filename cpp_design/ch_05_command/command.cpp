#include <iostream>
#include <string>
#include "command.h"

using namespace std;
// TODO: implement with variadics!
// TODO: without Loki typelist does not work

//////////////////////////////////////////////////////////////////////////
void show_simple_functor(){

    struct test_functor0 {
        void operator()(){
            cout << "operator()" << endl;
        }
    };

    struct test_functor1 {
        void operator()(int i){
            cout << "operator(" << i << ")" << endl;
        }
    };

    struct test_functor2 {
        void operator()(int i, double d){
            cout << "operator(" << i << "," << d << ")" << endl;
        }
    };

    test_functor0 f0;
    functor<void> fu0(f0);
    fu0();

    test_functor1 f1;
    functor<void, int> fu1(f1);
    fu1(2);

    test_functor2 f2;
    functor<void, int, double> fu2(f2);
    fu2(2, 3.5);
}

//////////////////////////////////////////////////////////////////////////
void test_function0(){
    cout << "test_function0()" << endl;
}

void test_function1(int i){
    cout << "test_function1(" << i << ")" << endl;
}

void test_function2(int i, double d){
    cout << "test_function2(" << i << "," << d << ")" << endl;
}

void test_function2(const char* s, double d){
    cout << "test_function2(" << s << "," << d << ")" << endl;
}

void show_simple_function(){
    functor<void> fu0(&test_function0);
    fu0();

    functor<void, int> fu1(&test_function1);
    fu1(2);

    // overloaded functions work directly well
    test_function2(2, 3.0);
    test_function2("atatat", 3.0);

    // however we can't use it even with the direct list of params
    // error: No constructor could take the source type, or constructor overload resolution was ambiguous
    //functor<void, int, double> fu2(&test_function2);
    //fu2(2, 3.5);

    // Apply direct casting to function type, it works now
    typedef void(*two_params_f)(int, double);
    functor<void, int, double> fu21(static_cast<two_params_f>(&test_function2));
    fu21(2, 3.5);
}
//////////////////////////////////////////////////////////////////////////
class parrot{
public:
    void eat(){
        cout << "Om nom nom" << endl;
    }
    void speak(){
        cout << "Piasters! Piasters!" << endl;
    }
};


void show_method_pointer(){

    typedef void(parrot::* parrot_method)();

    // Object itself
    parrot geronimo;
    parrot* p_geronimo = &geronimo;

    // Methods
    parrot_method activity = &parrot::eat;

    // call by value
    (geronimo.*activity)();

    // call by pointer
    (p_geronimo->*activity)();

    // geronimo.*activity (p_geronimo->*activity) not a type, just some entity of tow addresses -
    // object (stack, heap or static) and method (static)

    // change activity
    activity = &parrot::speak;

    // other pair of calls
    (geronimo.*activity)();
    (p_geronimo->*activity)();

    // Functor
    functor<void> fu0(&geronimo, &parrot::eat);
    functor<void> fu1(&geronimo, &parrot::speak);
    fu0();
    fu1();
}

int main(){
    show_simple_functor();
    show_simple_function();
    show_method_pointer();
    return 0;
}
