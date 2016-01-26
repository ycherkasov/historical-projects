#include <iostream>
#include "singleton.h"

using namespace std;

struct my_single_class{
    int a = 99;
    void increment(){
        ++a;
    }
};

struct my_longlive_single_class{
    int b = 100;
    void decrement(){
        --b;
    }
};

struct my_life_controlled_class1 : public lifetime_interface{
    int c = 100;
    virtual size_t get_longevity(){
        return 1;
    }
};

struct my_life_controlled_class2 : public lifetime_interface{
    int d = 110;
    virtual size_t get_longevity(){
        return 2;
    }
};

void show_simple_singleton(){

    // Simple singletons
    cout << singleton_holder<my_single_class>::instance().a << endl;
    singleton_holder<my_single_class>::instance().increment();
    cout << singleton_holder<my_single_class>::instance().a << endl;
}

void show_phoenix_singleton(){
    // These singletons will be re-created once if they accessed after destruction
    cout << singleton_holder<my_single_class, new_creation_policy, phoenix_lifetime>::instance().a << endl;
    singleton_holder<my_single_class, new_creation_policy, phoenix_lifetime>::instance().increment();
    cout << singleton_holder<my_single_class, new_creation_policy, phoenix_lifetime>::instance().a << endl;
}

void show_lifetime_control(){
    // Create 2 singletons with different lifetime
    // One is always destroyed first
    cout << singleton_holder<my_life_controlled_class1, new_creation_policy, longevity_control>::instance().c << endl;
    cout << singleton_holder<my_life_controlled_class2, new_creation_policy, longevity_control>::instance().d << endl;
}

int main(){
    show_simple_singleton();
    show_phoenix_singleton();
    show_lifetime_control();
    return 0;
}
