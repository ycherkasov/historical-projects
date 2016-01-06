#include <iostream>
#include <typeinfo>

using namespace std;

/*
New features:
* Uniform and general initialization using {}-lists (2.2.2, 6.3.5)
* Type deduction from initializer: auto (2.2.2, 6.3.6.1)
* Prevention of narrowing (2.2.2, 6.3.5)
* Generalized and guaranteed constant expressions: constexpr (2.2.3, 10.4, 12.1.6)
* Range-for-statement (2.2.5, 9.5.1)
* Null pointer keyword: nullptr (2.2.5, 7.2.2)
* Scoped and strongly typed enums: enum class (2.3.3, 8.4.1)
* Compile-time assertions: static_assert (2.4.3.3, 24.4)

1. Basic types (architecture-denendent and independent)
2. Constant (const, constexpr)
3. Curly braces init
4. Auto, auto examples, auto & generic programming
5. For, range-for
6. enum, enum class
7. Class invariant (negative length example)
8. Static assertions

*/

// 1. Basic types (architecture-dependent and independent)
// 2. Constant (const, constexpr)
// 3. Curly braces init

// declare as constexpr so that calc in compile-time
constexpr double test_me(int i1, int i2){
    return i1 + i2;
}

void show_basic_types(){

    // new initialisators
    double d1 = 2.3;
    double d{ 2.3 };

    // The = form is traditional and dates back to C, but if in doubt, 
    // use the general{}-list form.
    // If nothing else, it saves you from conversions that lose information
    // (narrowing conversions)

    // error: conversion from 'double' to 'int' requires a narrowing conversion
    // int i1{ 7.2 };

    // const have to be initialized
    const int ic = 17;
    const double dc = 10.0;
    // constexpr evaluates in compile time that all parts of expression is const
    // used as additional check or for the performance

    constexpr int ce_itn1 = 18;
    constexpr int ce_itn2 = 18 + 12;

    // test_me() is constexpr
    constexpr double ce_d1 = test_me(ce_itn1, ce_itn2);

    cout << typeid(ce_d1).name() << '\n';
    
    // error: expression did not evaluate to a constant
    // sqrt does not return constexpr
    // constexpr double d2 = sqrt(dc);

    // error - d1 is not const
    //constexpr double = sqrt(dc);
}

// 4. Auto, auto examples, auto & generic programming
void show_auto(){
    //We use auto where we don’t have a specific reason to mention the type explicitly. Specific reasons include:
    //* The definition is in a large scope where we want to make the type clearly visible to readers of our code.
    //* We want to be explicit about a variable’s range or precision (e.g.,doublerather thanfloat)

    auto b = true;
    cout << typeid(b).name() << '\n';

    auto ch = 'x';
    cout << typeid(ch).name() << '\n';

    auto l = 123;
    cout << typeid(l).name() << '\n';

    auto d = 1.23;
    cout << typeid(d).name() << '\n';

    auto s = "string";
    cout << typeid(s).name() << '\n';

    auto expr = sqrt(2.5);
    cout << typeid(expr).name() << '\n';
}

// 5. For, range-for
void show_for(){
    int arr[] = { 1, 2, 3, 4, 5, 6 };

    // range-for-statement
    // for loops that traverse a sequence

    // by value
    for (auto x : arr){
        cout << x << endl;
    }

    // by reference
    for (auto& x : arr){
        cout << x << endl;
    }

    // array emplaced
    for (auto x : { 10,21,32,43,54,65 }){
        cout << x << '\n';
    }
}

// 6. enum, enum class
void show_enum(){

    // Strong-typed enum
    // enum classes - enumerator names are local to the enum 
    // and their values do not implicitly convert to other types 
    // (like another enum or int)
    enum class Color{
        red,
        green,
        yellow
    };

    // Can not be assigned other type
    
    // ok
    Color c = Color::red;
    c = static_cast<Color>(1);
    
    // 'initializing': cannot convert from 'show_enum::Color' to 'int'
    // int i = Color::red;

    // classic enum
    enum TrafficLight{
        red = 1,
        yellow = 2,
        green = 3
    };
    TrafficLight tl = red;
    
    // Conversion to enumeration type requires an explicit cast (static_cast, C-style cast or function-style cast)
    //tl = Color::red;
    tl = static_cast<TrafficLight>(Color::red);
    tl = static_cast<TrafficLight>(1);

    // ok
    int i = TrafficLight::red;

    // understand even without qualifier (do not conflict with enum class)
    i = red;
}

// Invariant is a checked statement in moment of object creation
// (should be done in constructor)
// e.g. length should be non-negative

void show_static_assert(){

    // compile-time check (statement should be true)
    static_assert(4 <= sizeof(int), "Integers are too small");
}


// Such a statement of what is assumed to be true for a class 
// is called a class invariant, or simply an invariant

// is the program’s return value to the system.
// If no value is returned, the system will receive a value indicating successful completion.
// A nonzero value from main() indicates failure.
int main() {
    show_basic_types();
    show_auto();
    show_for();
    show_enum();
    return 0;
}
