#include <iostream>
#include "view.h"
#include "view_grains.h"

void view::event1()
{
    std::cout << "view::event1()" << std::endl;
}

void view::event2()
{
    std::cout << "view::event2()" << std::endl;
}

void view::drwaing1()
{
    std::cout << "view::drwaing1()" << std::endl;
}

void view::drwaing2()
{
    std::cout << "view::drwaing1()" << std::endl;
}

view::operator view_events*(){
    return new view_events(this);
}

view::operator view_drawings*(){
    return new view_drawings(this);
}
