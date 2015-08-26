#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <functional>

// From Satter 2. Ch.01 Templates

// 1.2-3 Strings, sprintf, C++ streams

// Dangerous version without buffer size change
void old_sprintf(int i, char* buf){

    // It contains following weaks:
    // 1. Buffer size is unchecked
    // 2. Dangerous formatting using flags
    // if we mistype and printed %4ld, we use long int, which size could differ then int
    // 3. It does not support type safety and could not be used with templated
    sprintf(buf, "%4d", i);
}

void new_snprintf(int i, char* buf, size_t len){
    // does not compile on Windows
    //snprintf(buf, len, "%4d", i);
}

// The most memory-safe and type-safe, but less efficient solution
const std::string new_stringstream(int i){
    std::ostringstream stream;
    stream << std::setw(4) << i;
    return stream.str();
}

void show_string_formatting(){


    char buf[5] = {};
    old_sprintf(42, buf);
    
    // Error: stack corrupted!
    //pretty_format(123456, buf);

    // does not compile on Windows
    //new_snprintf(42, buf, 5);

    std::string s = new_stringstream(42);
}

// Satter 2 1.4 mem-fun and standard library functions
void show_mem_fun(){
    // using pointers to functions of STD is dangeous because the signature may be changed
    auto pointer_to_clear = std::mem_fun( &std::vector<int>::clear );
}

// Satter 2 1.5 Templates

// Q: Why do we use 2 template params instead just T?
template <typename T1, typename T2>
void construct(T1* p, const T2& val){
    new (p) T1(val);
}

template <typename T>
void construct_unsafe(T* p, const T& val){
    new (p)T(val);
}

// A: because T2 might not be the same as T1, bit could be castable
// int-double, base-derived. See below

void show_plecament_new_template(){
    
    struct Base{ 
        virtual ~Base(){}; 
    };

    struct Derived : public Base{};

    Base b;
    Derived d;

    double* pd = new double;
    Base* pb = new Derived;

    construct(pd, 2.718);
    construct_unsafe(pd, 2.718);

    construct(pb, b);
    construct_unsafe(pb, b); // ok, b size less of equal d 

    construct(pd, 42); // - OK
    //construct_unsafe(pd, 42); - error, different types
    construct_unsafe<double>(pd, 42); // OK

    construct(pb, d); // OK
    //construct_unsafe(pb, d); // Error - different types
    construct_unsafe<Base>(pb, d); // OK

    delete pd;
    delete pb;
}


// Satter 2 1.6 Templates

// Q: Where's the problem in template?
template <typename T>
void my_destroy(T* p){
    p->~T();
}

template <typename FwdIter>
void my_destroy(FwdIter first, FwdIter last){
    while (first != last){
        my_destroy(first);
        ++first;
    }
}

// A: In the inner function we use instantiation for T* - pointers only
// In outer - for any iterator. It could be not only pointer, but object
// See example with std::list below

// Solution:
template <typename FwdIter>
void my_destroy2(FwdIter first, FwdIter last){
    while (first != last){
        // Trick - use operator* to bet object from iterator
        // use operator& to get its real address
        my_destroy(&*first);
        ++first;
    }
}

void show_iterator_instantiation_problem(){

    struct Base{
        virtual ~Base(){};
    };

    struct Derived : public Base{};

    Derived d1, d2;

    
    std::list<Derived> l{ d1, d2 };
    std::vector<Derived> v{ d1, d2 };

    // error C2784 : 'void my_destroy(T *)' : could not deduce template argument for 'T *' 
    // from 'std::_List_iterator<std::_List_val<std::_List_simple_types<show_iterator_instantiation_problem::Derived>>>'
    //my_destroy(l.begin(), l.end());
    
    // More efficient implementation
    my_destroy2(l.begin(), l.end());
}

// Q: What's the problem of example
template <typename T>
void my_swap(T& a, T& b){
    T temp(a);
    a = b;
    b = temp;
}
// A: exception safety


// Satter 2 1.7 Templates
// Functions template specializations and overloads
// Function template could be
// a) fully specialized (not partially!)
// b) overloaded with the function

// Q: which of the fuctions willbe called
template <typename T>
void f1(T){
    std::cout << "void f1(T)\n";
}

// complete specialization (not an overload, never take part in overload!)
template <>
void f1<int*>(int*){
    std::cout << "void f1<int*>(int*)\n";
}

// This in NOT a partial specialization, but separate primary template
// It overloads f1(T) but not f1<int*>
template <typename T>
void f1(T*){
    std::cout << "void f1(T*)\n";
}

// Overload template and functions:
// 1.Functions are always more privileged then templates
// 2.It there are no functions, primary templates are considered
// 3.ONLY after choosing primary template, its specializations are started to consider 

// The reason why this decision was made: "It would be surprising if in case of implementation some
// template specialization used primary specialization would has been changed"

// Passing parameters from function to class - emulate specialization, including partial

// fwd declare
template <typename T>
struct FImpl;

// Function without specializations or overloads
template <typename T>
void my_function(T t){
    FImpl<T>::f(t);
}

// Implementation class - could be partially specialized
template <typename T>
struct FImpl{
    static void f(T t){}
};

// Satter 2 1.8 Templates and friends
// Always use complete form with namespace and template parameters
class T{
public:
    // a bit sophisticated, but...
    friend class std::vector<T>;
};

void show_template_overloads(){

    // In that case we do the following:
    // 1.Look for plain function - no
    // 2.Look for primary templates - T and T*. Choose T*
    // 3.Look for specializations - no
    // call void f1(T*)
    int* p = nullptr;
    f1(p);
}

int main(){

    show_string_formatting();

    show_mem_fun();

    show_plecament_new_template();

    show_iterator_instantiation_problem();

    show_template_overloads();

    return 0;
}
