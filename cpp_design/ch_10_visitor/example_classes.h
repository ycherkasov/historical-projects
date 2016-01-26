#pragma once
#include <string>

class doc_element_visitor;

class doc_element{
public:
    virtual ~doc_element(){}

    // all references non-const - we can change visitor
    // visitor can change elements
    virtual void accept_visitor(doc_element_visitor&) = 0;
private:
};

// Element of document that containing text
// Have font size attribute
class text_paragraph : public doc_element{
public:

    text_paragraph(const char* txt) : text_(txt){}

    virtual void accept_visitor(doc_element_visitor&);

    size_t size() const { return text_.size(); }

    void set_font_size(size_t size);

private:
    std::string text_;
};

// Element of document that containing image
// Does not hold any text
class raster_bitmap : public doc_element{
public:
    raster_bitmap(int h) : bitmap_handle_(h){}
    virtual void accept_visitor(doc_element_visitor&);
private:
    int bitmap_handle_;
};

// Task: organize iteration so that collect information or do some changes
// Bad solution: add a new operation for every entry (CollectStats, ChangeFont etc.)
// It push us to know about all operations and all helper objects (StatCollector etc.)
