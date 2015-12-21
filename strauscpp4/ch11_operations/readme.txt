Questions:

New C++14 features:
* Lambdas (3.4.3,11.4)
* Local classes as template arguments (11.4.2, 25.2.1)

Advices:

* int size could be checked with static assert so that work with masks
* Whenever possi- ble, use standard-library facilities in preference to fiddling with pointers and bytes
* Wherever possible, have that manager object be a scoped variable
* The type of a {}-list can be deduced (only) if all elements are of the same type
* Unfortunately, we do not deduce the type of an unqualified list for a plain template argument
* The body of the lambda simply becomes the body of the operator()()
* lambda is called a closure object (or simply a closure)
* If a lambda potentially captures every local variable by reference (using the capture list [&]), the
  closure may be optimized to simply contain a pointer to the enclosing stack frame
* Naming the lambda is often a good idea
* If you need to capture a variadic template (28.6) argument, use ...
* A lambda might outlive its caller. Use copy capure then [=]
* We don’t need to 'capture' namespace variables (including global variables) because they are always accessible
* We can include class members in the set of names potentially captured by adding this
* Members are always captured by reference
* In the unlikely event that we want to modify the state, we can declare the lambda mutable
* If a lambda body does not have a return-statement, the lambda’s return type is void
* To allow for optimized versions of lambda expressions, the type of a lambda expression is not defined
* so no two lambdas have the same type
* we can use it to initialize a variable declared auto or std::function<R(AL)> 
  where R is the lambda’s return type and AL is its argument list
* Truncation of floating-point numbers is not 'well behaved'
* If rounding is desirable, we can use the standard- library function round()
* make type conversion more visible and to allow the programmer to express the intent of a cast:

The main problems with free store are:
* Leaked objects: People use new and then forget to delete the allocated object.
* Premature deletion: People delete an object that they have some other pointer to and later
use that other pointer
* Double deletion

The implementation model for {}-lists comes in three parts:
* If the {}-list is used as constructor arguments, the implementation is just as if you had used a
()-list. List elements are not copied except as by-value constructor arguments.
* If the {}-list is used to initialize the elements of an aggregate (an array or a class without a constructor),
 each list element initializes an element of the aggregate. 
 List elements are not copied except as by-value arguments to aggregate element constructors.
* If the {}-list is used to construct an initializer_list object
 each list element is used to initialize an element of the underlying array of the initializer_list. 
 Elements are typically copied from the initializer_list to wherever we use them

A unqualified list is used where an expected type is unambiguously known.It can be used as an expression only as :
* A function argument
* A return value
* The right - hand operand of an assignment operator (=, +=, *=, etc.)
* A subscript
