#include "small_object.h"

small_object_allocator small_object::alloc_(DEFAULT_CHUNK_SIZE, MAX_SMALL_OBJECT_SIZE);

void* small_object::operator new(size_t size)
{
    return alloc_.allocate(size);
}

void small_object::operator delete(void* p, size_t size)
{
    return alloc_.deallocate(p, size);
}

small_object::~small_object(){}
