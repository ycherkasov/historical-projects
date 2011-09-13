#include "static1/file2.h"
#include <iostream>

// int x определена в file2.cpp, но используется в качестве внешней в file2.cpp
int x = 1;

// константы и typedef компонуются внутри единицы трансляции
// поэтому могут быть определены в нескольких cpp-файлах
const int y = 1;
typedef char T;


void show_extern()
{
	std::cout << x;
}
