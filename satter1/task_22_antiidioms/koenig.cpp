#include <iostream>

// Task 9.1-4 Satter 1
// Catches and anti-idioms

class anti_idiom_collection{
public:

    // Plain constructor
    anti_idiom_collection() : _test(){}

    virtual ~anti_idiom_collection(){}

    // Some developers use such an useless check
    anti_idiom_collection(const anti_idiom_collection& other){
        
        // this check is useless, because the object still not created
        // However, it could make sense in some unusual cases (see show_unusual())
        if (this != &other){
            _test = other._test;
        }

        // Also, we may deny getting object address by overload operator&
    }

    // one more really unsafe anti-idiom
    anti_idiom_collection& operator=(const anti_idiom_collection& other){
        
        // This is a very unsafe code
        // 1. Slice object if anti_idiom_collection is a base class
        // 2. Exception-unsafe (see previous example)
        // 3. It brakes RAII semantic
        // 4. It does not handle derived classes
        // 5. This is a hack
        if (this != &other){
            this->~anti_idiom_collection();
            new (this) anti_idiom_collection(other);
        }
        return *this;
    }

private:
    int _test;
};

void show_unusual(){
    anti_idiom_collection a;
    
    // incorrect placement new
    new (&a) anti_idiom_collection(a);

    // incorrect assignment
    anti_idiom_collection a1 = a1;
}

void show_unsafe1(){

    // this is correct, but unsafe code
    anti_idiom_collection a1;
    anti_idiom_collection& ra = a1;
    a1.~anti_idiom_collection();
    
    // exception could be thrown here
    // anti_idiom_collection() won't be called
    // ~anti_idiom_collection() will be be called twice
    new (&a1) anti_idiom_collection;
}

void show_unsafe2(){
    anti_idiom_collection a1;
    anti_idiom_collection a2;
    a2 = a1;
}

int main(){

    show_unusual();

    show_unsafe1();

    show_unsafe2();

    return 0;
}
