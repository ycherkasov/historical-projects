#pragma once

// Visitor is useful when a lot of classes in well-built hierarchy
// with a lot of virtual functions
// It is hard to add a new function, but it is easy to add a new class

// It consists of two hierarchies - inspected, where we don't want to add functions
// and inspector (visitor) that contain hierarchy of operations

class text_paragraph;
class raster_bitmap;

class doc_element_visitor{
public:
    // must be statically mentioned all inspected types
    virtual void visit(text_paragraph&) = 0;
    virtual void visit(raster_bitmap&) = 0;
};

// Visitor that collects document statistics
class collect_stat_visitor : public doc_element_visitor{
public:
    // must be statically mentioned all inspected types
    virtual void visit(text_paragraph&);
    virtual void visit(raster_bitmap&);
    void display_statistics();
private:
    size_t symbols_ = 0;
    size_t images_ = 0;
};

// Visitor that change font size
class change_font_size_visitor : public doc_element_visitor{
public:

    change_font_size_visitor(size_t s) :new_font_size_(s){}

    // must be statically mentioned all inspected types
    virtual void visit(text_paragraph&);
    virtual void visit(raster_bitmap&);
private:
    size_t new_font_size_;
};

// Alexandrescu also mention class AcyclicVisitor that allows not to include
// information of inspected classes to simple_visitor.cpp
// I don't like it as it has a lot of dynamic_cast checks instead and looks like a hack
