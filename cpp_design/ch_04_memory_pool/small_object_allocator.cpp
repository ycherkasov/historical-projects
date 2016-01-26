#include "small_object_allocator.h"
#include <cassert>
#include <algorithm>

small_object_allocator::small_object_allocator(size_t chunk_size, size_t max_object_size) :
last_alloc_(nullptr),
last_dealloc_(nullptr),
chunk_size_(chunk_size),
max_object_size_(max_object_size){}


void* small_object_allocator::allocate(size_t num_bytes){

    if (num_bytes > max_object_size_){
        return operator new(num_bytes);
    }

    if (last_alloc_ && last_alloc_->block_size() == num_bytes){
        return last_alloc_->allocate();
    }

    auto it = std::lower_bound(pool_.begin(), pool_.end(),
        num_bytes, [](const fixed_allocator& al, size_t num_bytes){

        return al.block_size() > num_bytes;
    });

    if (it == pool_.end() || it->block_size() == num_bytes){
        it = pool_.insert(it, fixed_allocator(num_bytes));
        last_dealloc_ = &*pool_.begin();
    }

    last_alloc_ = &*it;
    return last_alloc_->allocate();
}


void small_object_allocator::deallocate(void* p, size_t num_bytes){

    if (num_bytes > max_object_size_){
        return operator delete(p);
    }

    if (last_dealloc_ && last_dealloc_->block_size() == num_bytes){
        last_dealloc_->deallocate(p);
        return;
    }

    auto it = std::lower_bound(pool_.begin(), pool_.end(),
        num_bytes, [](const fixed_allocator& al, size_t num_bytes){

        return al.block_size() > num_bytes;
    });

    assert(it != pool_.end());
    assert(it->block_size() == num_bytes);

    last_dealloc_ = &*it;
    last_dealloc_->deallocate(p);
}
