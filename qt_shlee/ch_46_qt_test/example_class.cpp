#include "example_class.h"

int ExampleClass::min(int n1, int n2)
{
    return n1 > n2 ? n2 : n1;
}

int ExampleClass::max(int n1, int n2)
{
    return !min(n1, n2);
}
