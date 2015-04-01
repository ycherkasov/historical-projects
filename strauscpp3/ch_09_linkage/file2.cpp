#include "file2.h"
#include <iostream>

// int x определена в file2.cpp, но используется в качестве внешней в file1.cpp
int x = 1;

// константы и typedef компонуются внутри единицы трансляции
// поэтому могут быть определены в нескольких cpp-файлах
const int y = 1;
typedef char T;

// "i" has external linkage.
namespace{
	int i;
}
// This is correct.  Even though "i" is not visible outside the compilation unit, 
// it still has external linkage (C++ Standard 7.3.1.1/1 - See footnote).

void show_extern()
{
	std::cout << x << std::endl;
}
