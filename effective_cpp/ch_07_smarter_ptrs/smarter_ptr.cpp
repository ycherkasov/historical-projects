#include <memory>
#include "view.h"
#include "view_grains.h"

/*
Chapter 07 Jeff Eldger

* smarter (interface) pointers are able to work like COM interfaces, i.e. provide an interface subset
* aggregation of private inheritance could be used for that
*/

using namespace std;

class view_events;
class view_drawings;

// Something similar is implemented in MS COM technology
void show_interface_ptr(){
    // object itself could be hidden (or used as a private base)
    view my_view;

    std::unique_ptr<view_events> ev(my_view);
    ev->event1();
    ev->event2();

    std::unique_ptr<view_drawings> dr(my_view);
    dr->drawing1();
    dr->drawing2();
}

int main(){
    show_interface_ptr();
    return 0;
}
