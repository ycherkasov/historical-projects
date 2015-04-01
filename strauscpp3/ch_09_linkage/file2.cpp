#include "file2.h"
#include <iostream>

// int x ���������� � file2.cpp, �� ������������ � �������� ������� � file1.cpp
int x = 1;

// ��������� � typedef ����������� ������ ������� ����������
// ������� ����� ���� ���������� � ���������� cpp-������
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
