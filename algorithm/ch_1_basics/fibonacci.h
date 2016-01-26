#ifndef _FIBONACCI_H_
#define _FIBONACCI_H_

// VERY ineffective implementation based on direct recursion 
// O(exp(n)) complexity
void fibonacci_naive();

// more effective implementation with 2-values cache
// O(n) complexity for machine types, O(n^2) complexity for long integers
// (including operations )
void fibonacci_effective();

// compile-time implementation using templates
// O(1) complexity for n in cache size
void fibonacci_metaprogramming();

typedef unsigned long long uint64_t;

template<uint64_t stage>
struct Fib
{
    //Make this value a constant value equal to the (stage-1) + (stage -2)
    //which the compile will generate for us and save in the types of:
    // Fib<stage-1> and Fib<stage-2>. This all works because stage is known at compile 
    // time, as all template parameters must be.
    static const uint64_t value = Fib<stage-1>::value + Fib<stage-2>::value;

    static inline uint64_t getValue(unsigned i)
    {
        if (i == stage) // Does the current class hold the given place?
        {
            return value;  // Return it!
        } else {
            return Fib<stage-1>::getValue(i); // Get it from the previous class!
        }
    }
};

template<> // Template specialization for the 1's case.
struct Fib<1>
{
    static const uint64_t value = 1;

    static inline uint64_t getValue(unsigned i)
    {
        if(i == 0)
            return 0;
        return 1;
    }
};

template<> // Template specialization for the 2's case
struct Fib<2>
{
    static const uint64_t value = 1;

    static inline uint64_t getValue(unsigned i)
    {
        if (i == 2)
        {
            return value;
        } 
        else {
            return Fib<1>::getValue(i);
        }
    }
};

#endif//_FIBONACCI_H_