#include "user_allooc.h"
#include "construct.h"

#include <iostream>
#include <cstdio>
#include <new>

intern g_intern;

/** @brief 
2 ����� ��������� new
1 - �������(show_new1)
2 - � ����������(show_new2 - i � j ����� ���������� ������)

*/

void show_new1() {
	// ������ �������� ������
	int* i = new int(6);
	printf("%d\n", *i);
	delete i;
}


void show_new2() {
	int * i = new int(12);
	printf("*i = %d\n", *i);

	// �������������� ������������ ������, � ������ ������ i = j
	int * j = new(i) int(7);   // ���������� new
	printf("*j = %d\n", *j);
	printf("*i = %d\n", *i);
	delete i;   // ����� �� ������� � j
}

user_allooc::user_allooc(void)
{
	std::cout << "user_allooc()" << std::cout;
}

user_allooc::~user_allooc(void)
{
	std::cout << "~user_allooc()" << std::cout;
}

void* user_allooc::operator new(size_t s, void* p) throw(){
	//return _alloca(s);
	return p;
}

void user_allooc::operator delete(void* p){
	//free(p);
}

// new_op_new.cpp
// compile with: /EHsc
#include<new>
#include<iostream>

using namespace std;

class MyClass 
{
public: 
	MyClass( ) : imember()
	{
		cout << "Construction MyClass." << this << endl;
	};

	~MyClass( )
	{
		imember = 0; cout << "Destructing MyClass." << this << endl;
	};
	int imember;
};

void show_new_delete()
{
	// ������, ������������ ����� ��������� new
	// � ������ ������� ������ ���������� bad_alloc
	MyClass* fPtr = new MyClass;
	delete fPtr;

	// ������, ���������� ������� new
	// �������� ������ �� ������������� ������
	// �� ������� ������������ ������
	// ������������ ����� ��� ����������������� ������� ������������ ������������. 
	// ������ ���������� ��� ������ � �������� ������� ������ ��������.
	char x[sizeof( MyClass )];
	MyClass* fPtr2 = new( x ) MyClass;
	fPtr2->~MyClass();
	cout << "The address of x[0] is : " << ( void* )&x[0] << endl;
	// http://rsdn.ru/forum/cpp/3679187.aspx

	//���������� ����������� � ������������������ ��������, 
	//��� ����� ������ �������� �� ����������� �������� � ������. 
	//���, ��������, ���� ������������ ������, �.�. ����� �� ������ ���������� �����, 
	//������� ������ ������������, � ������� ��� ��������.

/*	� ������ ����������������� ���� ��������� ����������� ��������� new, 
	� � ����� ������ ����� ��� ��������������� �������������� �������� ���������� new/delete.
	�������� � ���, ��� ��������� new/delete ������ �� �����. 
	��������, ��� ������ � ������� ��������� overhead ����� ���� ������� �������, 
	���� �� ���� ������������ ������ (�.�. ������� ������ ��������) 
	����� ������ ������������������ ��� ���������� ������ ������. 
	���� �� �� ������, ��� � ��� ��������� ������ ��� ���� ������� 
	(� �� ������ �� ������������ �����, ��� ��� ������������������ �������� �������� ������),
	�� �� ������ ���������� �� ������ �������� ���� ������� ����� ������ 
	(��� ��� ������� �����, ������ ��� ������ ���� ��������) ��� ���� ����� ��������, 
	� ����������� �������� new/delete ��� ������ ��� �
	����������� ���������� "��������". 
	����� �������� �� ������ �������������� ������ ������ � ������� ������� ������������������.
	*/

	// ������, ���������� ����� ��������� new
	// � ������ ������� ���������� 0
	MyClass* fPtr3 = new( nothrow ) MyClass;
	if(fPtr3){ /*do something*/ }
	delete fPtr3;
}

