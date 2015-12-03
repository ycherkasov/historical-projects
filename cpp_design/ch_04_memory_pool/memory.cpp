#include <iostream>
#include "small_object.h"

// Andrei Alexandrescu. Chapter 4. Memory pool
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

    for (size_t i = 0; i < 1024; ++i){
        std::cout << "Created size: " << sizeof(small_class1) << std::endl;
        small_class1* s1 = new small_class1;
        small_classes1.push_back(s1);
    }

    for (size_t i = 0; i < 1024; ++i){
        small_class1* s1 = small_classes1[i];
        delete s1;
    }
}

int main(){
    test_allocation();
    return 0;
}
