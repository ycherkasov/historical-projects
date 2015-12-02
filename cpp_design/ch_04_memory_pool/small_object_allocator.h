#pragma once
#include "fixed_allocator.h"


class small_object_allocator {
public:

    /// chunk_size - become a default size for every chunk created by allocator
    /// max_object_size - objects whose size is below that are considered like 'small'
    small_object_allocator(size_t chunk_size, size_t max_object_size);

    void* allocate(size_t num_bytes);

    void deallocate(void* p, size_t block_size);

private:

    // pool_[i] saves all allocated objects of size i
    std::vector<fixed_allocator> pool_;

    // last allocator (same size) who has been used for allocation
    fixed_allocator* last_alloc_;

    // last allocator (same size) who has been used for deallocation
    fixed_allocator* last_dealloc_;

    size_t chunk_size_;
    size_t max_object_size_;
};
