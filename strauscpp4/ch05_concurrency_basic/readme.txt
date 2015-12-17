Questions:

New C++14 features:

* Resource management pointers: unique_ptr, shared_ptr, and weak_ptr (5.2.1)
* Concurrency support: thread (5.3.1), mutexes (5.3.4), locks (5.3.4), and condition variables (5.3.4.1)
* Higher-levelconcurrencysupport:packaged_thread,future,promise,andasync()(5.3.5)
* tuples (5.4.3)
* Regular expressions: regex (5.5)
* Random numbers

Advices:
 
* Use resource handles to manage resources (RAII)
* Use unique_ptr to refer to objects of polymorphic type
* Use shared_ptr to refer to shared objects
* Use type-safe mechanisms for concurrency
* Minimize the use of shared data
* Don’t choose shared data for communication because of 'efficiency' without thought and preferably not without measurement
* Think in terms of concurrent tasks, rather than threads
* A library doesn't have to be large or complicated to be useful
* Time your programs before making claims about efficiency
* You can write code to explicitly depend on properties of types
* Use regular expressions for simple pattern matching
* Don't try to do serious numeric computation using only the language, use libraries
* Properties of numeric types are accessible through numeric_limits

* unique_ptr is moved, shared_ptr is copied
* We do not need to use a pointer to return a collection of objects, use containers instead