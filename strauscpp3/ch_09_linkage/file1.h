#pragma once

void use_extern();

// ���������� �� ������ extern "C" ����������� ��� ������������ ��������������
// ����� C++ � �/FORTRAN/Assembler etc.
extern "C" void c_function1();

// ����� ��������� ���������� ���
extern "C"{
	void c_function2();
	void c_function3();

	// ��� �������� ��������� �� ������� ��������� �� C
	// ����� ������� ��������� ���������� � ����������
	typedef void(qsort_t)(void*, size_t, size_t);
	// ����� ��� �������� ����� ��������� ��������������� �-��� 
	// ����� ������������ ��� ��
	// void i_sort(qsort_t sort_function); - ����� ������������ � "C"
};

// ����� �������� ����� ����
extern "C"{
#include "string.h"
};

// �� ����� ���
#ifdef __cplusplus
#define EXTERNC  extern "C"
#else
#define EXTERNC 
#endif

