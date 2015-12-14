Questions:

New C++14 features:
{}-lists	(2.2.2)
auto		(2.2.2)
prev narrowing	(2.2.2)
constexpr	(2.2.3)
range-for	(2.2.5)
nullptr		(2.2.5)
enum/enum class	(2.2.3)
static_assert	(2.4.3)

Advices:
* please don’t assume that 'the old ways' are better or simpler just because they are old and familiar
* Use const to make variable non-changed, constexpr to check whether all elements of expression is const (could be used for safety and performance)
* A constant cannot be left uninitialized
* Use 'auto' instead of long types, especially in generic programming
* use new form of range-for
* use 'enum class' for strong type safety
* use 'static_assert' for compile-time checks
* C++ provided no support for multiple address spaces
* 