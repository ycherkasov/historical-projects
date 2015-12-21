#include <iostream>

using namespace std;

/*

New features:
* Null pointer keyword (7.2.2)
* Rvalue references (enabling move semantics)(7.7.2)
* Unicode characters (6.2.3.2, §7.3.2.2)
* Raw string literals (7.3.2.1)
* Scoped and strongly typed enums: enum class (2.3.3, 8.4.1)
* Generalized POD (8.2.6)
* Generalized unions (8.3.1)
* A fixed-sized contiguous sequence container: array (8.2.4, 34.2.1)
* Generalized and guaranteed constant expressions: constexpr (2.2.3, 10.4, 12.1.6)
* Range-for-statement (2.2.5, 9.5.1)

Examples:
1. nullptr!= NULL; Rvalue references, move.
2. Unicode and raw strings, backslash, LR
3. Union tag
4. Explicit enum class base, plane and unnamed enums
5. Literal type (constexpr constructor)

*/

void show_ptrs_refs() {}

void show_stringLiterals() {}

void show_union_tags() {}

void show_enum_classes() {}

void show_literal_types() {}

int main() {
    show_ptrs_refs();
    show_stringLiterals();
    show_union_tags();
    show_enum_classes();
    show_literal_types();

    return 0;
}
