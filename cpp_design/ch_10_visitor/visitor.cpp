#include <iostream>
#include <vector>
#include "example_classes.h"
#include "simple_visitor.h"

using namespace std;


void show_simple_visitor(){

    vector<doc_element*> my_document;

    my_document.push_back(new text_paragraph("First paragraph"));
    my_document.push_back(new raster_bitmap(1));
    my_document.push_back(new text_paragraph("Second paragraph"));

    // collect statistics
    collect_stat_visitor stat;
    for (doc_element* el : my_document){
        el->accept_visitor(stat);
    }
    stat.display_statistics();

    // change font size
    change_font_size_visitor font_ch(12);
    for (doc_element* el : my_document){
        el->accept_visitor(font_ch);
    }


    // cleanup
    for (doc_element* el : my_document){
        delete el;
    }
}

int main(){
    show_simple_visitor();
    return 0;
}
