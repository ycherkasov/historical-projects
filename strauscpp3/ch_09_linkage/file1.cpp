#include "file1.h"

// int x ���������� � file2.cpp, �� ������������ � �������� ������� � file1.cpp
extern int x;
extern int x; // � ��� �� ������, �.�. extern ������ ������ �� ���������!

// ������� show_extern ����� ���������� � file2.cpp
extern void show_extern();

// ��������� � typedef ����������� ������ ������� ����������
// ������� ����� ���� ���������� � ���������� cpp-������
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