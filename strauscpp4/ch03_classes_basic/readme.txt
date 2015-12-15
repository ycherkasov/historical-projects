Questions:

New C++14 features:

initializer_list
rvalue-references
tmpl args ending
lambdas
variadic
type/template aliases
initializer_list containers
move semantic

Advices:

* Express ideas directly in code
* Define classes to represent application concepts directly in code
* Use concrete classes to represent simple concepts and performance-critical components
* Avoid 'naked' new and delete operations
* Use resource handles and RAII to manage resources
* Use abstract classes as interfaces when complete separation of interface and implementation is needed
* Use class hierarchies to represent concepts with inherent hierarchical structure
* When designing a class hierarchy, distinguish between implementation inheritance and interface inheritance
* Control construction, copy, move, and destruction of objects
* Return containers by value (relying on move for efficiency)
* Provide strong resource safety; that is, never leak anything that you think of as a resource
* Use containers, defined as resource handle templates, to hold collections of values of the same type
* Use function templates to represent general algorithms
* Use function objects, including lambdas, to represent policies and actions
* Use type and template aliases to provide a uniform notation for types that may vary among similar types or among implementations

* virtual call mechanism can be made almost as efficient as the 'normal function call' (within 25%)
* In very much the same way as new and delete disappear from application code, we can make pointers disappear into resource handles