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

void show_simple_singleton(){

    cout << singleton_holder<my_single_class>::instance().a << endl;
    singleton_holder<my_single_class>::instance().increment();
    cout << singleton_holder<my_single_class>::instance().a << endl;
}

void show_phoenix_singleton(){
    cout << singleton_holder<my_single_class, new_creation_policy, phoenix_lifetime>::instance().a << endl;
    singleton_holder<my_single_class, new_creation_policy, phoenix_lifetime>::instance().increment();
    cout << singleton_holder<my_single_class, new_creation_policy, phoenix_lifetime>::instance().a << endl;
}

void show_lifetime_control(){
}

int main(){
    show_simple_singleton();
    show_phoenix_singleton();
    return 0;
}
