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