Questions:
* Logic errors vs runtime errors
* what is nested_exception
* system_errors some article
* default_error_condition()
* http://stackoverflow.com/questions/21821673/understanding-the-system-error-facility-in-c11

New C++14 features:
* Copying and rethrowing exceptions (30.4.1.2)
* Error reporting using error codes: system_error (30.4.3)

Advices:
* The .h equivalents to <cstdbool>, <ccomplex>, <calign>, and <ctgmath> approximate C++ facilities for C
* A programmer can specialize utility templates, such as swap(), for non-stan- dard-library, user-defined types
* A {}-list is converted into an object of type std::initializer_list<X>
* The standard library style and approaches to error handling are not consistent:
	* C-style set errno
	* end() as not found
	* state flags (streams)
	* exceptions
	* system_error
* For STL even if an exception is thrown, no resource (such as memory) is leaked and no invariant for a standard-library class is broken
* Do not throw built-in types, such as int and C-style strings. Instead, throw objects of types specifically defined to be used as exceptions
* Logic errors are errors that in principle could be caught either before the program starts executing or by tests
  of arguments to functions and constructors. Run-time errors are all other errors
* it is a good idea to somewhere catch all exceptions
* An exception_ptr can point to any exception, not just exceptions from the exception hierarchy. 
  Think of exception_ptr as a smart pointer (like shared_ptr) that keeps its exception alive for 
  as long as an exception_ptr points to it
* exception_ptr can be used to implement a re-throw of an exception in a different thread from the one in which the exception was caught
  This is what promise and future rely on
* A nested_exception is class that stores an exception_ptr obtained from a call of current_exception()
* An exception cannot propagate out of a noexcept function
* I consider uncaught_exception() too subtle for practical use
* The error message produced by assert() is implementation-defined but should contain 
  the source file name (__FILE__), and the source line number (__LINE__)
* In <system_error>, the standard library provides facilities for classifying error codes, 
  for mapping system-specific error codes into more portable ones, and for mapping error codes into exceptions
	* error_code: Holds a value identifying an error and the category of that error. System-specific
	* error_category: A base class for types used to identify the source and encoding of a particular kind (category) of error code
	* system_error: A runtime_error exception containing an error_code
	* error_condition: Holds a value identifying an error and the category of that error. Potentially portable
* For a type representing the simple idea of an error code, error_code provides a lot of members
  It is basically as simple map from an integer to a pointer to an error_category
* An error_category is an interface to an object of a class derived from error_category
  Therefore, an error_category is passed by reference and stored as a pointer
* The operations on error_codes are system-specific. In some cases, error_codes can be mapped into error_conditions
* An error_category represents a classification of errors
  Specific errors are represented by a class derived from class error_category
* Because error_category is designed to be used as a base class, no copy or move operations are provided
* A system_error is used to report errors that ultimately originate in the parts of the standard library that deal with the operating system
* Naturally, system_errors can be used by code that is not part of the standard library
  A system-specific error_code is passed, rather than a potentially portable error_condition
* To get an error_condition from an error_code use default_error_condition()
* The general idea is that each system has a set of specific (native) codes that are mapped 
  into the potentially portable ones for the convenience of programmers of programs 
  (often libraries) that need to work on multiple platforms
* Making an error_category with a set of error_codes and at least one error_condition 
  starts with defining an enumeration with the desired error_code values
  The meaning of these values is completely category-specific
* Argument intended to become the value() of an error_code of future_category() must be a future_errc
  In particular, we can’t just use any int
* To help the implementer of error_code, we specialize the trait is_error_code_enum for our enumeration
* Standard error_codes for the system_category() are defined by enum class errc with values equivalent to the POSIX-derived contents of <cerrno>
* The POSIX macros are integers whereas the errc enumerators are of type errc
* 

Book advices: 