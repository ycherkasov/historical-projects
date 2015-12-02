#pragma once
#include <cstdint>
#include <vector>

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

class fixed_allocator{
public:

    /// Init sizes and alignment, set linked list of fixed_allocators
    fixed_allocator(size_t block_size);
    ~fixed_allocator();


    fixed_allocator(const fixed_allocator& other);
    fixed_allocator& operator=(const fixed_allocator& other);

    /// Allocate from existing pool or enlarge pool for 50% and allocate
    void* allocate();

    // Deallocate a memory block previously allocated with allocate()
    // (if that's not the case, the behavior is undefined)
    void deallocate(void* p);

    // accessor for overlaying classes
    size_t block_size() const {
        return block_size_;
    }

private:

    // Chunk - base fixed-size internal allocator
    struct chunk
    {
        /// Create memory pool
        void init(size_t block_size, size_t blocks);

        /// Actual size so that allocate more than one memory chunk
        /// No search have O = const
        void* allocate(size_t block_size);

        /// Set block from pool as free
        void deallocate(void* p, size_t block_size);

        /// Release all the pool
        void release();

        /// actual pool
        /// First byte of every FREE block saves index of the NEXT FREE block
        /// Every block consists on 4 smaller blocks
        uint8_t* data_;

        /// available for allocation (max sizeof(uint8_t))
        uint8_t blocks_available_;

        /// integer index of the first available block
        uint8_t first_available_;
    };

    // Finds the chunk corresponding to a pointer, using an efficient search
    chunk* find_deallocated(void* p);

    // ...we already know the chunk, deallocate p from it
    void do_deallocate(void* p);


private:
    // pass these params to chunk
    size_t block_size_;
    uint8_t num_blocks_;

    // set of memory pools
    std::vector<chunk> chunks_;

    // current chunk we work with (for faster search of free block)
    chunk* alloc_chunk_;

    // The LAST chunk where we was deleting from
    // (for efficient deallocation search algorithm)
    chunk* dealloc_chunk_;

    // For ensuring proper copy semantics
    mutable const fixed_allocator* prev_;
    mutable const fixed_allocator* next_;
};
