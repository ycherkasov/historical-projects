#include "static1/file2.h"
#include <iostream>

// int x ���������� � file2.cpp, �� ������������ � �������� ������� � file2.cpp
int x = 1;

// ��������� � typedef ����������� ������ ������� ����������
// ������� ����� ���� ���������� � ���������� cpp-������
const int y = 1;
typedef char T;


void show_extern()
{
	std::cout << x;
}
