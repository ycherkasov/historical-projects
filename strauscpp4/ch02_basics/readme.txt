Questions:
1. The difference between 'enum' and 'enum class'

Advices:
* Use const to make variable non-changed, constexpr to check whether all elements of expression is const
(could be used for safety and performance)
* Use 'auto' instead of long types, especially in generic programming
* use new form of range-for
* use 'enum class' for strong type safety
* use 'static_assert' for compile-time checks