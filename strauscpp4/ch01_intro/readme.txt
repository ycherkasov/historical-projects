îáùèå ðåêîìåíäàöèè

* use virtual functions instead of "if" and "case"
* use STL/Boost algorithms, functors and lambdas instead of cycles
* use smart pointers (unique_ptr, shared_ptr) instead of new/delete
* don't use C arays, prefer containers (e.g. std::vecror is array-compatible &v[0])
* think of complicated classes copying (move semantic, swap method)
* always define copy constructor and assignment operator (private if not required)
* Don't use C-casts, C++ only (static_cast, const_cast è ò.ä.),
* remember that inheritance inherits more interfaces than implementation
* don't believe compiler too much ;-)

Advices:
* Try to use Ñ++11
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



Here are what I consider the most widely useful new ‘‘building bricks’’ affecting the style of
C++11 code with references to the text and their primary authors:

• Control of defaults: =deleteand =default: §3.3.4, §17.6.1, §17.6.4; Lawrence Crowl and
Bjarne Stroustrup.
* Deducing the type of an object from its initializer,auto: §2.2.2, §6.3.6.1; Bjarne Stroustrup.
I first designed and implementedautoin 1983 but had to remove itbecause of C compatibility problems.
* Generalized constant expression evaluation (including literal types), constexpr: §2.2.3,
§10.4, §12.1.6; Gabriel Dos Reis and Bjarne Stroustrup [DosReis,2010].
* In-class member initializers: §17.4.4; Michael Spertus and Bill Seymour.
* Inheriting constructors: §20.3.5.1; Bjarne Stroustrup, Michael Wong, and Michel Michaud.
* Lambda expressions, a way of implicitly defining function objects at the point of their use in
an expression: §3.4.3, §11.4; Jaakko Jarvi.
* Move semantics, a way of transmitting information without copying: §3.3.2, §17.5.2;
Howard Hinnant.
* A way of stating that a function may not throw exceptions noexcept: §13.5.1.1; David Abrahams, Rani Sharoni, and Doug Gregor.
* A proper name for the null pointer, §7.2.2; Herb Sutter and Bjarne Stroustrup.
* The range-forstatement: §2.2.5, §9.5.1; Thorsten Ottosen and Bjarne Stroustrup.
* Override controls:finalandoverride: §20.3.4.  Alisdair Meredith, Chris Uzdavinis, and Ville
Voutilainen.
* Type aliases, a mechanism for providing an alias for a type or a template. In particular, a
way of defining a template by binding some arguments of another template: §3.4.5, §23.6;
Bjarne Stroustrup and Gabriel Dos Reis.
* Typed and scoped enumerations: enum class: §8.4.1; David E. Miller, Herb Sutter, and
Bjarne Stroustrup.
* Universal and uniform initialization (including arbitrary-length initializer lists and protection against narrowing): §2.2.2, §3.2.1.3, §6.3.5, §17.3.1, §17.3.4; Bjarne Stroustrup and
Gabriel Dos Reis.
* Variadic templates, a mechanism for passing an arbitrary number of arguments of arbitrary
types to a template: §3.4.4, §28.6; Doug Gregor and Jaakko Jarvi.
