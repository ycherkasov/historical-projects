#include <cstdlib>
#include <iostream>
#include "static2/file2.h"

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
