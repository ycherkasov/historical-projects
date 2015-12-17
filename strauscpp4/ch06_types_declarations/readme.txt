Questions:

New C++14 features:
* Uniform and general initialization using {}-lists (2.2.2, 6.3.5)
* Type deduction from initializer: auto (2.2.2, 6.3.6.1)
* Prevention of narrowing (2.2.2, 6.3.5)
* Type and template aliases (3.4.5, 6.5, 23.6)
* Unicode characters (6.2.3.2, 7.3.2.2)
* long long integer type (6.2.4)
* Alignment controls: alignas and alignof(6.2.9)
* The ability to use the type of an expression as a type in a declaration: decltype 
* C99 features (long long)
* Integer type names, such as int16_t, uint32_t, and int_fast64_t (6.2.8)

Advices:
 
* Many important things are deemed implementation-defined by the standard
* Other behaviors are unspecified;  the implementer is not obliged to specify which actually occur
* For example, the exact value returned by new is unspecified
* A construct is deemed undefined by the standard if no reasonable behavior is required by an implementation
* A C++ implementation can be either hosted or freestanding
* Freestanding implementations are meant for code running with only the most minimal operating system support
* The Boolean, character, and integer types are collectively called integral types
* In arithmetic and logical expressions, bools are converted to ints
* A pointer can be implicitly converted to a bool

Scopes:
* Local scope: A name declared in a function  is called a local name
* Class scope: A name is called a member name  if it is defined in a class outside any function
* Namespace scope: A name is called a namespace member name if it is defined in a namespace  
  outside any function, lambda , or other namespace
* Global scope: A name is called a global name if it is defined outside any function, class , or namespace
* Statement scope: A name is in a statement scope if it is defined within the  part of a for-, while-, if-, or switch-statement
