#include "file1.h"

// int x определена в file2.cpp, но используетс€ в качестве внешней в file1.cpp
extern int x;
extern int x; // и это не ошибка, т.к. extern ничего нового не объ€вл€ет!

// функци€ show_extern также определена в file2.cpp
extern void show_extern();

// константы и typedef компонуютс€ внутри единицы трансл€ции
// поэтому могут быть определены в нескольких cpp-файлах
const int y = 2;
typedef  int T;

extern const int x1 = 10;
//It defines a global constant variable that can be referenced from other modules.
//This is correct (C++ Standard 7.1.1/6).

void use_extern(){
	show_extern();
	x = 10;
}

void c_function1(){
	// do something
}

void c_function2(){
	// do something
}

void c_function3(){
	// do something
}