#pragma once

class view;

// Class that provides a subset of the 'view' interface
class view_events
{
public:
    view_events(view* v) : view_impl_(v){}

    void event1();

    void event2();

private:
    view* view_impl_;
};

// Class that provides other subset of the 'view' interface
class view_drawings
{
public:
    view_drawings(view* v) : view_impl_(v){}

    void drawing1();

    void drawing2();

private:
    view* view_impl_;
};
