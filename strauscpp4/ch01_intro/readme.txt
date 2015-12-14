TODO: Move Ñ/C++ difference to the related topic

New C++14 features:
None

Old advices:
* use virtual functions instead of 'if' and 'case'
* use STL/Boost algorithms, functors and lambdas instead of cycles
* use smart pointers (unique_ptr, shared_ptr) instead of new/delete
* don't use C arays, prefer containers (e.g. std::vecror is array-compatible &v[0])
* think of complicated classes copying (move semantic, swap method)
* always define copy constructor and assignment operator (private if not required)
* Don't use C-casts, C++ only (static_cast, const_cast),
* remember that inheritance inherits more interfaces than implementation
* don't believe compiler too much ;-)
* use for input istream_iterator/ostream_iterator
* istream_iterator() default constructor creates pointer to eof
* use fstream for files

Advices:
* Try to use C++11
* There's no room for more low-level language than C++
* What you don't use you don't pay for
* Express ideas in code, not in comments
* Express independent ideas in code independently
* Represent relations between ideas as interfaces in the code
* Only these C++ expressions requires runtime support: new, delete, typeid, dynamic_cast, throw/try/catch
* C++ don't provide support for multiple address spaces
* prefer static type check
* keep scope of view minimal
* don't overabstract (don't generalize too much)
* don't use C++ as plain C (it's often less optimal)
* use const/constexpr enum/enum class to manifest constants
* use void* unions, casts in low-level code only
* use noexcept() instead of exceptions specifications
* Use constructors to establish invariants
* Avoid 'naked' new and delete
* Use move semantics to avoid copying large objects 



Here are what I consider the most widely useful new 'building bricks' affecting the style of C++11 code:

* Control of defaults: =deleteand =default
* Deducing the type of an object from its initializer,auto
* Generalized constant expression evaluation (including literal types), constexpr
* In-class member initializers
* Inheriting constructors
* Lambda expressions, a way of implicitly defining function objects at the point of their use in an expression
* Move semantics, a way of transmitting information without copying
* A way of stating that a function may not throw exceptions noexcept
* A proper name for the null pointer
* The range-forstatement
* Override controls:final and override
* Type aliases, a mechanism for providing an alias for a type or a template. 
* In particular, a way of defining a template by binding some arguments of another template
* Typed and scoped enumerations: enum class
* Universal and uniform initialization (including arbitrary-length initializer lists and protection against narrowing)
* Variadic templates, a mechanism for passing an arbitrary number of arguments of arbitrary types to a template

Ñ/C++ difference

http://en.wikipedia.org/wiki/Compatibility_of_C_and_C%2B%2B
* C allows a void* pointer to be assigned to any pointer type without a cast
int *j = malloc(sizeof(int) * 5);

* C++ prohibit using goto or switch jumping over an initialization
void fn(void)
{
  goto flack;
  int i = 1;
flack:
  return;
}

* C does not allow a given typedef to be duplicated in the same scope, whereas C++ allows repeated typedef
* enum values are always of type int in C, whereas they are distinct types in C++ and may have a size different from that of int
* C++ also changes some C standard library functions to add additional functions with const qualifiers
* in C++, a nested struct is defined only within the scope/namespace of the outer struct
* Non-prototype ("K&R"-style) function declarations are not allowed in C++
* In C, a function prototype without arguments, e.g. int foo(), implies that the parameters are unspecified
* C++ is more strict than C about pointer assignments that discard a const qualifier
* C++ allows to use const variable in case: operator, C doesn't
* character literals such as 'a' are of type int in C and of type char in C++
* C++ implicitly treats any const global as file scope unless it is explicitly declared extern, 
  unlike C in which extern is the default
* In C++, an object declaration is also a definition unless it contains an extern specifier and no initializer
* Conversely, inline functions in C are of file scope whereas they have external linkage by default in C++
* C requiring struct in front of structure tags (and so sizeof(T) refers to the variable), but C++ allowing it to be omitted

extern int T;
int size(void)
{
    struct T {  int i;  int j;  };
 
    return sizeof(T);
    /* C:   return sizeof(int)
     * C++: return sizeof(struct T)
     */
}

* in C99, a new keyword, _Bool, is introduced as the new boolean type
* C compilers do not name mangle symbols in the way that C++ compilers do
* C can recursively call main()
  C++ 5.2.2/9
  Recursive calls are permitted, except to the function named main (3.6.1).
* Variable param number macros
* Pragma in macros
* _Complex & _Imaginary types
* restrict qualifier let compiler to know that we have only one pointer to that memory block
* Call the function that declared after the call (if would be found by the linker)
* variable-sized arrays
void function(int size)
{
   char a[size];
}

