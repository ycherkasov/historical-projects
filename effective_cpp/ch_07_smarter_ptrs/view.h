#pragma once

class view_events;
class view_drawings;

// Some class or other entity with a lot of interface functions
class view
{
    // we can also set constructor protected and inherit privately from grains
#if 0
protected:
    view(){}
#endif

public:
    // a lot of functions
    // Part 1 of interface (events)
    void event1();

    void event2();

    // Part 2 of interface (drawings)
    void drwaing1();

    void drwaing2();

    operator view_events*();
    operator view_drawings*();

private:
};
