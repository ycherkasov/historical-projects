#include <iostream>
#include "simple_visitor.h"
#include "example_classes.h"

using namespace std;

void collect_stat_visitor::visit(text_paragraph& p)
{
    symbols_ += p.size();
}

void collect_stat_visitor::visit(raster_bitmap& b)
{
    ++images_;
}

void collect_stat_visitor::display_statistics()
{
    cout << "Symbols in document: " << symbols_ << endl;
    cout << "Images in document: " << images_ << endl;
}

void change_font_size_visitor::visit(text_paragraph& p)
{
    p.set_font_size(new_font_size_);
}

void change_font_size_visitor::visit(raster_bitmap& b)
{
    // do nothing with picture
}
