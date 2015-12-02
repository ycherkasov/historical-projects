#include <iostream>
#include "small_object.h"

#pragma pack(4)

// Andrei Alexandresku. Chapter 4. Memory pool
struct small_class1 : public small_object
{
    int i = 0;
};

struct small_class2 : public small_object
{
    int i = 0;
    int j = 1;
};

struct small_class3 : public small_class2
{
    int i1 = 2;
    int j1 = 3;
};

void test_allocation(){

    std::vector<small_class1*> small_classes1;
    std::vector<small_class2*> small_classes2;

    for (size_t i = 0; i < 2; ++i){
        small_class1* s1 = new small_class1;
        small_classes1.push_back(s1);
    }

    for (size_t i = 0; i < 2; ++i){
        delete small_classes1[i];
    }
}

int main(){
    test_allocation();
    return 0;
}
