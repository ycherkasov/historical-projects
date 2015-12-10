#include <iostream>
#include "static_init.h"

//static
int static_initializer::hard_to_init_resourse = 0;

//static
size_t static_initializer::static_counter = 0;


static_initializer::static_initializer()
{
    ++static_counter;
    check_init_resourses();
}

static_initializer::~static_initializer()
{
    --static_counter;
    check_init_resourses();
}

void static_initializer::init_resourses()
{
    std::cout << "Allocate heavy recourses" << std::endl;
    hard_to_init_resourse = 100500;
}

void static_initializer::deinit_resourses()
{
    std::cout << "De-allocate heavy recourses" << std::endl;
    hard_to_init_resourse = 0;
}

void static_initializer::check_init_resourses()
{
    if (static_counter == 1){
        init_resourses();
    }
    if (static_counter == 0){
        deinit_resourses();
    }
}
