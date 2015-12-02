#include "fixed_allocator.h"
#include <cassert>
#include <memory>
#include <algorithm>
#include <functional>

void fixed_allocator::chunk::init(size_t block_size, size_t blocks = sizeof(uint8_t)){

    // allocate pool
    data_ = new uint8_t[block_size*blocks];

    // all blocks available
    blocks_available_ = blocks;
    first_available_ = 0;
    uint8_t* p = data_;

    // assign free blocks index (as build-in one-directional list)
    // this allows to find a free block in a const time
    for (uint8_t i = 0; i != blocks; p += block_size) {
        *p = ++i;
    }
}

void* fixed_allocator::chunk::allocate(size_t block_size){

    if (0 == blocks_available_){
        return nullptr;
    }

    uint8_t* result = data_ + (first_available_ + block_size);

    // assign index of the first available block
    first_available_ = *result;

    --blocks_available_;

    return result;
}

void fixed_allocator::chunk::deallocate(void* p, size_t block_size){

    assert(p >= data_);
    uint8_t* release_me = static_cast<uint8_t*>(p);

    // alignment check
    assert((release_me - data_) % block_size == 0);

    first_available_ = static_cast<uint8_t>((release_me - data_) / block_size);

    // check for slicing
    assert((release_me - data_) / block_size);

    ++blocks_available_;
}

void fixed_allocator::chunk::release()
{
    delete[] data_;
}

//////////////////////////////////////////////////////////////////////////

fixed_allocator::fixed_allocator(size_t block_size) :
block_size_(block_size),
alloc_chunk_(nullptr),
dealloc_chunk_(nullptr)
{
    prev_ = this;
    next_ = this;

    size_t num_blocks = DEFAULT_CHUNK_SIZE / block_size_;
    if (num_blocks > UCHAR_MAX) {
        num_blocks = UCHAR_MAX;
    }
    else if (num_blocks == 0){
        num_blocks = 8 * block_size_;
    }

    num_blocks_ = static_cast<unsigned char>(num_blocks);
    assert(num_blocks_ == num_blocks);
}

fixed_allocator::fixed_allocator(const fixed_allocator& other) :
block_size_(other.block_size_),
num_blocks_(other.num_blocks_),
chunks_(other.chunks_)
{
    // fix linked list
    prev_ = &other;
    next_ = other.next_;
    other.next_->prev_ = this;
    other.next_ = this;

    // bring the active alloc chunk from copied chunks list (same offset)
    alloc_chunk_ = other.alloc_chunk_
        ? &chunks_.front() + (other.alloc_chunk_ - &other.chunks_.front())
        : nullptr;

    // bring the active deallocate chunk from copied chunks list (same offset)
    dealloc_chunk_ = other.dealloc_chunk_
        ? &chunks_.front() + (other.dealloc_chunk_ - &other.chunks_.front())
        : nullptr;
}

fixed_allocator& fixed_allocator::operator=(const fixed_allocator& other) {

    fixed_allocator cp(other);
    std::move(cp);
    return *this;
}


fixed_allocator::~fixed_allocator(){

    if (prev_ != this){
        prev_->next_ = next_;
        next_->prev_ = prev_;
        return;
    }

    // only one in the list
    assert(prev_ == next_);
    std::for_each(chunks_.begin(), chunks_.end(), std::mem_fun_ref(&chunk::release));
}


void* fixed_allocator::allocate(){

    // current block is not available
    if (nullptr == alloc_chunk_ || 0 == alloc_chunk_->blocks_available_){

        // current chunk is completely used, find appropriate
        auto available_chunk = std::find_if(chunks_.begin(), chunks_.end(), [](chunk& ch){
            return ch.blocks_available_ > 0;
        });

        // available chunk found
        if (available_chunk != chunks_.end()){
            alloc_chunk_ = &*available_chunk;
        }
        // available chunk not found add a new one, mass adding new memory to the pool
        else{
            size_t s = chunks_.size() / 2;
            size_t add_to_pool = s ? s : 1;
            chunks_.reserve(chunks_.size() + add_to_pool);
            chunks_.emplace_back(chunk());
            chunks_.back().init(block_size_, num_blocks_);

            // make first added chunk active
            alloc_chunk_ = &chunks_.back();
            dealloc_chunk_ = &chunks_.back();

            // just add other chunks (they are POD)
            for (size_t i = 0; i < add_to_pool - 1; ++i){
                chunks_.emplace_back(chunk());
                chunks_.back().init(block_size_, num_blocks_);
            }
        }
    }

    return alloc_chunk_->allocate(block_size_);
}

fixed_allocator::chunk* fixed_allocator::find_deallocated(void* p)
{
    assert(!chunks_.empty());
    assert(dealloc_chunk_);

    // so that know the range where to look the pointer
    const size_t chunk_length = num_blocks_ * block_size_;

    // look for the deallocated pointer widen owr search borders
    chunk* lower_start_search = dealloc_chunk_;
    chunk* higher_start_search = dealloc_chunk_ + 1;
    chunk* lower_bound = &chunks_.front();
    chunk* higher_bound = &chunks_.back() + 1;

    // Special case: dealloc_chunk_ is the last in the array
    if (higher_start_search == higher_bound) {
        higher_start_search = 0;
    }

    for (;;)
    {
        if (lower_start_search) {

            if (p >= lower_start_search->data_ && p < lower_start_search->data_ + chunk_length) {
                return lower_start_search;
            }

            if (lower_start_search == lower_bound) {
                lower_start_search = 0;
            }
            else {
                --lower_start_search;
            }
        }

        if (higher_start_search) {

            if (p >= higher_start_search->data_ && p < higher_start_search->data_ + chunk_length){
                return higher_start_search;
            }
            if (++higher_start_search == higher_bound) {
                higher_start_search = 0;
            }
        }
    }
    assert(false);
    return 0;
}

void fixed_allocator::do_deallocate(void* p){

    assert(dealloc_chunk_->data_ <= p);
    assert(dealloc_chunk_->data_ + num_blocks_ * block_size_ > p);

    // call into the chunk, will adjust the inner list but won't release memory
    dealloc_chunk_->deallocate(p, block_size_);

    // special case - all chunk is available
    if (dealloc_chunk_->blocks_available_ == num_blocks_) {

        // dealloc_chunk_ is completely free, should we release it?
        chunk& last_chunk = chunks_.back();

        if (&last_chunk == dealloc_chunk_) {

            // check if we have two last chunks empty
            if (chunks_.size() > 1 && dealloc_chunk_[-1].blocks_available_ == num_blocks_) {

                // Two free chunks, discard the last one
                last_chunk.release();
                chunks_.pop_back();
                alloc_chunk_ = dealloc_chunk_ = &chunks_.front();
            }
            return;
        }

        if (last_chunk.blocks_available_ == num_blocks_) {

            // Two free blocks, discard one
            last_chunk.release();
            chunks_.pop_back();
            alloc_chunk_ = dealloc_chunk_;
        }
        else {
            // move the empty chunk to the end
            std::swap(*dealloc_chunk_, last_chunk);
            alloc_chunk_ = &chunks_.back();
        }
    }
}

void fixed_allocator::deallocate(void* p) {

    assert(!chunks_.empty());
    assert(&chunks_.front() <= dealloc_chunk_);
    assert(&chunks_.back() >= dealloc_chunk_);

    // find chunk where the deallocated pointer is contained
    dealloc_chunk_ = find_deallocated(p);
    assert(dealloc_chunk_);

    // remove from it
    do_deallocate(p);
}
