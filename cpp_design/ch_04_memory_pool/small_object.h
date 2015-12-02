#pragma once
#include "small_object_allocator.h"

class small_object{
public:

    static void* operator new(size_t size);

    static void operator delete(void* p, size_t size);

    // TODO: switch virtually
    ~small_object();

private:
    static small_object_allocator alloc_;
};
