* use virtual functions instead of 'if' and 'case'
* use STL/Boost algorithms, functors and lambdas instead of cycles
* use smart pointers (unique_ptr, shared_ptr) instead of new/delete
* don't use C arays, prefer containers (e.g. std::vecror is array-compatible &v[0])
* think of complicated classes copying (move semantic, swap method)
* always define copy constructor and assignment operator (private if not required)
* Don't use C-casts, C++ only (static_cast, const_cast),
* remember that inheritance inherits more interfaces than implementation
* don't believe compiler too much ;-)

Advices:
* Try to use C++11
* There's no room for more low-level language than C++
* What you don't use you don't pay for
* Express ideas in code, not in comments
* Express independent ideas in code independently
* Represent relations between ideas as interfaces in the code
* Only these C++ expressions requires runtime support:
  new, delete, typeid, dynamic_cast, throw/try/catch
* C++ don't provide support for multiple address spaces
* prefer static type check
* keep scope of view minimal
* don't overabstract (don't generalize too much)
* don't use C++ as plain C (it's often less optimal)
* use const/constexpr enum/enum class to manifest constants
* use void* unions, casts in low-level code only
* use noexcept() instead of exceptions specifications


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
