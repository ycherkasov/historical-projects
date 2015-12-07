#include <iostream>
#include "example_classes.h"
#include "simple_visitor.h"

using namespace std;

void text_paragraph::accept_visitor(doc_element_visitor& visitor){

    // visitor do something with the text paragraph
    visitor.visit(*this);
}

void text_paragraph::set_font_size(size_t size)
{
    cout << "Changing font size to " << size << endl;
}

void raster_bitmap::accept_visitor(doc_element_visitor& visitor){

    // visitor do something with the raster bitmap
    visitor.visit(*this);
}
