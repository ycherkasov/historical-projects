#include <iostream>

// From Satter 2. Ch.03 Memory

// Ch 3 Task 20 Memory Management levels

// 4 levels:
// 1. Operating system allocators
// 2. Low-level standard library allocators
// 3. Standard container allocators
// 4. Custom user allocators

// Ch 3 Task 21 Allocation and alignment

// 1, 4 or 8
#define ALIGN 8
#pragma pack(ALIGN)

void show_service_data(){

    // It we requested 'alloc_size' bytes it does not mean
    // that it will be allocated exactly 'alloc_size'
    // It means at least 'alloc_size' will be allocated
    const int alloc_size = 5;
    void* m_allocated = malloc(alloc_size);
    if (m_allocated == nullptr){
        std::cout << "Shit happen" << std::endl;
        return;
    }

    char* im_allocated = static_cast<char*>(m_allocated);
    
    std::cout << "malloc()" << std::endl;
    for (int i = -10; i < alloc_size+5; ++i){
        std::cout << "A[" << i << "] = " << static_cast<short>(im_allocated[i]) << '\n';
    }
    std::cout << std::endl;

    // we see that allocated memory wrapped with some service data
    free(m_allocated);

    std::cout << "Release" << std::endl;

    for (int i = -10; i < alloc_size + 5; ++i){
        std::cout << "A[" << i << "] = " << static_cast<short>(im_allocated[i]) << '\n';
    }
    std::cout << std::endl;

    std::cout << "new()" << std::endl;

    int* new_allocated = new int[alloc_size];
    for (int i = -10; i < alloc_size + 5; ++i){
        std::cout << "A[" << i << "] = " << new_allocated[i] << '\n';
    }
    std::cout << std::endl;

    // same with new
    delete[] new_allocated;

    // see also examples in Straus3
}

struct bad_alligned{
    char c1;
    long l1;
    char c2;
};

struct good_alligned{
    long l1;
    char c1;
    char c2;
};

void show_alignment(){
    bad_alligned bad;
    good_alligned good;

    std::cout << "Alignment = " << ALIGN << std::endl;
    std::cout << "sizeof(bad) = " << sizeof(bad) << std::endl;
    std::cout << "sizeof(good) = " << sizeof(good) << std::endl;
}

// Ch 3 Task 22 When overload at least one new/delete operator,
// overload all others that could be required new/new[], placement new/new[], related delete()
// This is because of hiding names (see Overload and hiding names and name resolve rules)

// Ch 3 Task 23 checking bad_cast make sense only in particular cases
// In some OSes memory is allocated according to "lazy evaluation" algo
// operator new() is always success, but the application may fail
// Solution: overload new(), fill memory with 0000
// (catch should-be platform-specific)

int main(){

    show_service_data();

    show_alignment();

    return 0;
}
