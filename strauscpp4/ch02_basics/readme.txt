Questions:
enum/enum class difference

New C++14 features:
* Uniform and general initialization using {}-lists (2.2.2, 6.3.5)
* Type deduction from initializer: auto (2.2.2, 6.3.6.1)
* Prevention of narrowing (2.2.2, 6.3.5)
* Generalized and guaranteed constant expressions: constexpr (2.2.3, 10.4, 12.1.6)
* Range-for-statement (2.2.5, 9.5.1)
* Null pointer keyword: nullptr (2.2.5, 7.2.2)
* Scoped and strongly typed enums: enum class (2.3.3, 8.4.1)
* Compile-time assertions: static_assert (2.4.3.3, 24.4)

Advices:
* A constant (2.2.3) cannot be left uninitialized
* With auto, we use the = syntax because there is no type conversion involved that might cause problems
* C++ supports two notions of immutability: const and constexpr
* New constructor initializes the members using a member initializer list
* The class after the enum specifies that an enumeration is strongly typed
* By default, an enum class has only assignment, initialization, and comparisons
* Such a statement of what is assumed to be true for a class is called a class invariant
* If an error can be found at compile time, it is usually preferable to do so

* please don’t assume that 'the old ways' are better or simpler just because they are old and familiar
* Use const to make variable non-changed, constexpr to check whether all elements of expression is const (could be used for safety and performance)
* A constant cannot be left uninitialized
* Use 'auto' instead of long types, especially in generic programming
* use new form of range-for
* use 'enum class' for strong type safety
* use 'static_assert' for compile-time checks
* C++ provided no support for multiple address spaces