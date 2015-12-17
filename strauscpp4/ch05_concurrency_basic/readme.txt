Questions:

New C++14 features:

* Resource management pointers: unique_ptr, shared_ptr, and weak_ptr (5.2.1)
* Concurrency support: thread (5.3.1), mutexes (5.3.4), locks (5.3.4), and condition variables (5.3.4.1)
* Higher-levelconcurrencysupport:packaged_thread,future,promise,andasync()(5.3.5)
* tuples (5.4.3)
* Regular expressions: regex (5.5)
* Random numbers

Advices:
 

* unique_ptr is moved, shared_ptr is copied
* We do not need to use a pointer to return a collection of objects, use containers instead