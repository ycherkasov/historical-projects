#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>

/*

New features:

Language mapping of {}-list to std::initializer_list (3.2.1.3)
Rvalue references (enabling move semantics; 3.3.2)
Nested template arguments ending with >> (no space between the > s; 3.4.1)
Lambdas(3.4.3)
Variadic templates (3.4.4)
Type and template aliases (3.4.5)
initializer_list constructors for containers (3.2.1.3)
Move semantics for containers (3.3.1)

1. Class complex
* {}-init

2. Class vector
* rvalue reference
* move semantic
* initializer_list constructors

3. Return unique_ptr
* move semantic

4. Lambdas

5. Variadic templates
* nested args

6. Type aliases


*/

namespace cpp4 {

// 1. Class complex
// Concrete class that behaves like a built-in type
class complex {
public:

    // constructor uses {} notation
    complex(double re = 0., double im = 0.) : re_{re}, im_{im}{}

    double real() const { return re_; }
    double imag() const { return im_; }

    // TODO: different operators...

private:
    double re_;
    double im_;
};


// 2. Class vector
template <typename T>
class vector {
public:

    // default constructor without any additional init
    // already inited at declaration
    vector(){}

    // The std::initializer_list used to define the initializer-list constructor 
    // Passed by value
    vector(std::initializer_list<T> l) : data_{new T[l.size()]} {
        // so that keep exception-safety
        std::copy(l.begin(), l.end(), data_);
        sz_ = l.size();
    }

    ~vector() {
        delete[] data_;
    }

    // copy semantic
    // sould be suppressed by = delete notation
    // it can be used to suppress any operation
    vector(const vector& rhs) : data_{new T[rhs.size()]} {
        // so that keep exception-safety
        std::copy(rhs.data_, rhs.data_ + rhs.size(), data_);
        sz_ = rhs.size();
    }

    // assign semantic
    // sould be suppressed by = delete notation
    // it can be used to suppress any operation
    vector& operator=(const vector& rhs) {

        if(&rhs != this) {
            // so that keep exception-safety
            T* data = nullptr;
            try {
                data = new T[rhs.size()];
                std::copy(rhs.data_, rhs.data_ + rhs.size(), data);
                delete data_;
                data_ = data;
                sz_ = rhs.size();
            }
            catch (...) {
                delete[] data;
                throw;
            }
        }
        return *this;
    }

    // move semantic
    // The && means 'rvalue reference' and is a reference to which we can bind an rvalue
    // A move constructor does not take a const argument: after all, 
    // a move constructor is supposed to remove the value from its argument.
    // A move assignment is defined similarly.
    vector(vector&& rhs) {
        data_ = rhs.data_;
        sz_ = rhs.sz_;
        rhs.data_ = nullptr;
        rhs.sz_ = 0;
    }
    // A move operation is not implicitly generated for a class 
    // where the user has explicitly declared a destructor.

    size_t size() const { return sz_; }

private:
    size_t sz_ = 0;
    T* data_ = nullptr;
};

// 3. Return unique_ptr
std::unique_ptr<int> ret_unique_ptr(){
    std::unique_ptr<int> p(new int(10));
    return p;
    // behaves like return move( p );

    // This elision of copy/move operations, called copy elision, is permitted
    // in a return statement in a function with a class return type, 
    // when the expression is the name of a non-volatile automatic object 
    // with the same cv - unqualified type as the function return type

    // So this should call copy construction, however calls move
    // and works like a new RVO
}

// 4. Functor
// One particularly useful kind of template is the function object (sometimes called a functor)
// A predicate is something that we can invoke to return true or false

// The notation [&](int a){ return a<x; } is called a lambda expression. 
// It generates a function object exactly like Less_than<int>{x}. 
// The [&] is a capture list specifying that local names used (such as x) 
// will be passed by reference. Had we wanted to 'capture' only x, we could have said so: [&x]
// Had we wanted to give the generated object a copy of x, we could have said so: [=x]. 
// Capture nothing is [], capture all local names used by reference is [&], 
// and capture all local names used by value is [=]


// 5. Variadics
// an arbitrary number of arguments of arbitrary types
// Should be at least 3 functions:
// * recursive call for all params
// * call for head param
// * end-of-recursion

template <typename T>
void process(T t) {
    std::cout << t << "=" << typeid(t).name() << std::endl;
}

template <typename T, typename... Tail>
void variadic_func(T head, Tail... tail) {
    process(head);
    variadic_func(tail...);
}

// close recursion
void variadic_func(){}

// 6. Type aliaces
// introduce a synonym for a type or a template

// as a synonim of typedef
using inch_t = unsigned long;

// The using syntax has an advantage when used within templates. 
// If you need the type abstraction, but also need to keep template parameter 
// to be possible to be specified in future. You should write something like this
template<typename C>
using Element_type = typename C::value_type;

template<typename Container> void algo(Container& c){
    
    vector<Element_type<Container>> vec;

    std::cout << vec.size() << std::endl;
}

// another example
template<typename Value>
using string_map = std::map<Value, std::string>;


} // namespace cpp4


void show_complex() {
    cpp4::complex c1;
    cpp4::complex c2{ 1.0 };
    cpp4::complex c3{ 1.0, 2.0 };

    std::cout << c1.imag() << std::endl;
    std::cout << c2.imag() << std::endl;
    std::cout << c3.imag() << std::endl;
}

void show_vector() {
    cpp4::vector<int> v1{ 1,2,3,4,5 };
    std::cout << v1.size() << std::endl;
}

void show_return_by_move() {

    std::unique_ptr<int> p = cpp4::ret_unique_ptr();
    std::cout << *p << std::endl;
}

// TODO: show lambdas

void show_variadics() {
    cpp4::variadic_func(1, 1.2, "hello");
}

void show_type_aliaces() {
    std::vector<int> v;
    cpp4::algo(v);
}

int main() {
    show_complex();
    show_vector();
    show_variadics();
    show_type_aliaces();
    return 0;
}
