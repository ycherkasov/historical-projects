#include <memory>
#include "view.h"
#include "view_grains.h"

using namespace std;

class view_events;
class view_drawings;





// Something similar is implemented in MS COM technology
void show_interface_ptr(){
    // object itself could be hidden
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
