#include <iostream>

using namespace std;

// Andrei Alexandresku. Chapter 1. Strategies 


// Chapter one specialization of separate methods example

// With one param
template <typename T>
struct widget{

    void fun(){
        cout << "Widget<T>::Fun()" << endl;
    }
};

// Full spec of single members is available!
template <>
void widget<char>::fun(){
    cout << "Widget<char>::Fun()" << endl;
}

// With 1+ params
template <typename T, typename U>
struct gadget{

    void fun(){
        cout << "Gadget<T, U>::Fun()" << endl;
    }
};

// Partial spec of members is unavailable
#if 0
template <typename T>
void gadget<T, char>::fun(){
    cout << "Gadget<T, char>::Fun()" << endl;
}
#endif

// For the whole class is ok
template <typename T>
struct gadget<T, char>{

    void fun(){
        cout << "Gadget<T, char>::Fun()" << endl;
    }
};

void show_partial_member_spec(){

    widget<int> wi;
    widget<char> wc;
    wi.fun();
    wc.fun();

    gadget<int, double> gid;
    gid.fun();

    gadget<int, char> gic;
    gic.fun();

}

// Strategy and Traits
// Traits specialize types, strategy - behavior

// Examples:
// Creation strategies

// 1. 
template <typename T>
class new_stragegy{
    
    static T* create(){
        return new T;
    }
protected:
    // use only as a base
    ~new_stragegy();
};

// 2. 
template <typename T>
class malloc_stragegy{

    static T* create(){
        void* buffer = malloc(sizeof(T));
        if (!buffer)
            return nullptr;
        return new (buffer) T;
    }

};

// 3. Strategy could have different interface (unlike abstract class)
template <typename T>
class clone_stragegy{

    static T* create(){
        return prototype_->Clone();
    }

    void set_prototype(const T* prototype){
        prototype_ = prototype;
    }
private:
    T* prototype_;
};

// Template parameters of classes more likely then parameters of methods
// so that all class had access to types

// Strategy-by-strategy principle
// Strangest strategy could be constructed from weakest

// See example. Class uses some strategy CheckingPolicy
template <typename T, template <typename> class CheckingPolicy >
class smart_ptr : public CheckingPolicy<T> {
public:

    // template constructor for strategies casting
    // Only if 'CheckingPolicy<T>(other)' accepts casting!
    template <typename T1, template <typename> class CP1>
    smart_ptr(const smart_ptr<T1, CP1>& other) :
        CheckingPolicy<T>(other){}

};

int main(){

    show_partial_member_spec();

    return 0;
}
