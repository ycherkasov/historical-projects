#include <cstddef>
#include <cassert>
#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>

// Andrei Alexandresku. Chapter 4. Memory pool 


// Chunk - base fixed-size allocator
struct chunk
{
    /// Create memory pool
    void init(size_t block_size, size_t blocks);

    /// Actual size so that allocate more than one memory chunk
    /// No search have O = const
    void* allocate(size_t block_size);


    void deallocate(void* p, size_t block_size);
    
    /// actual pool
    /// First byte of every FREE block saves index of the NEXT FREE block
    /// Every block consists on 4 smaller blocks
    uint8_t* data_;

    /// available for allocation (max sizeof(uint8_t))
    uint8_t blocks_available_;

    /// integer index of the first available block
    uint8_t first_available_;
};

void chunk::init(size_t block_size, size_t blocks = sizeof(uint8_t)){
    
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

void* chunk::allocate(size_t block_size){
    
    if (0 == blocks_available_){
        return nullptr;
    }

    uint8_t* result = data_ + (first_available_ + block_size);
    
    // assign index of the first available block
    first_available_ = *result;
    
    --blocks_available_;

    return result;
}

void chunk::deallocate(void* p, size_t block_size){
    assert(p >= data_);
    uint8_t* release_me = static_cast<uint8_t*>(p);

    // alignment check
    assert( (release_me - data_) % block_size == 0 );

    first_available_ = static_cast<uint8_t>((release_me - data_) / block_size);

    // check for slicing
    assert((release_me - data_) / block_size);

    ++blocks_available_;
}

//////////////////////////////////////////////////////////////////////////

class fixed_allocator{
public:

    fixed_allocator();

    void* allocate();

private:
    // pass these params to chunk
    size_t block_size_;
    uint8_t num_blocks_;
    
    // set of memory pools
    std::vector<chunk> chunks_;

    // current chunk we work with (for faster search of free block)
    chunk* alloc_chunk_;

    chunk* dealloc_chunk_;
};


void* fixed_allocator::allocate(){

    // current block is ok, use it
    if (alloc_chunk_ && alloc_chunk_->blocks_available_){
        return alloc_chunk_->allocate(block_size_);
    }

    // current chunk is completely used, find appropriate
    auto available_chunk = std::find_if(chunks_.begin(), chunks_.end(), [](chunk& ch){
        ch.blocks_available_ > 0
    });

    // available chunk found
    if (available_chunk != chunks_.end()){
        alloc_chunk_ = &*available_chunk;
    }
    // available chunk not found add a new one
    else{
        chunks_.reserve(chunks_.size() + 1);
        chunks_.emplace_back(chunk());
        chunks_.back().init(block_size_, num_blocks_);
        alloc_chunk_ = &chunks_.back();
        dealloc_chunk_ = &chunks_.back();
    }

    return alloc_chunk_->allocate(block_size_);
}

int main(){
	
	return 0;
}