#include "user_allooc.h"
#include "construct.h"

#include <iostream>
#include <cstdio>
#include <new>

using namespace std;

intern g_intern;

// new �������� ������, �������� �����������, ���������� ��������� ������� ����.
// placement new ������� ������ ���, �������� ����� ������������ � ������� ��������� ������� ����.
// ���� �� �������� � �����������, �������� ������ ������� ��������� � ��� reinterpret_cast

/** @brief 
3 ����� ��������� new 
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
	cout << "user_allooc()" << endl;
}

user_allooc::~user_allooc(void)
{
	cout << "~user_allooc()" << endl;
}

void* user_allooc::operator new(size_t s) {
	
	void* p = 0;
	// ��� ���������� ��������� new ���� �������������� ���������� ������� ������
	// 1. �������� �� 0
	// ����������� ���� - ��������� 0 �� 1 ��� �����
	if(0 == s){
		s = 1;
	}

	// 2. ��������� �������� �������� ������
	while(true){
		p = alloca(s);
		if(p){
			return p;
		}
		else{
			// set_new_handler defines what to do in case of
			// lack of memory
			new_handler globalHandler = set_new_handler(0);
			if(0 == globalHandler){
				// �� �������� ����������, ������ ������ ����������
				throw bad_alloc();
			}
			else{

				set_new_handler(globalHandler);
				globalHandler();
			}

		}
	}

	return p;
}

void user_allooc::operator delete(void* p){
	// ������ 1 ������� 8
	// ��� operator delete() ���������� ������������� �������� �� ����
	if(0 == p)
		return;
	free(p);
}

// ������������� ����� ����������� ��������� new
void* user_allooc::operator new(size_t s, void* p){
	return p;
}

void user_allooc::operator delete(void* p, size_t s){
}

// ����� new � ���������� new_handler
void* user_allooc::operator new(size_t s, new_handler handler){
	new_handler oldHandler = set_new_handler(handler);
	void* p = alloca(s);
	if(p){
		return p;
	}
	// � ������ ������� ����� ������ ����� ����������
	return nullptr;
}

// new_op_new.cpp
// compile with: /EHsc

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
	if(fPtr3 == 0){
		/*error*/ 
	}
	delete fPtr3;
}


void no_more_memory(){
	cout << "no more memory: new_handler" << endl;
	// ������ 1 ������� 8
	//�� ����� ���� operator new() �������� �������� ������ ����� ������ ����,
	//�����������, ��� �������-���������� � ����������� ���-�� ���������� ������� ��� �� ������������

	// ���������� ���������� ����������, ������� �� ������ ������ ���������
	// 1. ���������� ���������� ������
	// 2. ����� �� ���������
	// 3. ���������� ���������� set_new_handler()
	// 4. ���������� set_new_handler(0) - ����� ����� ������ ������� ���������� bad_alloc

	// exit ������� �����������!
	exit(1);
}

// Mixtury class defenition that replaces standard
// new_handler
new_handler NewHandlerSupport::_current_handler = 0;

new_handler NewHandlerSupport::set_new_handler(new_handler p)
{
	new_handler old_handler = _current_handler;
	_current_handler = p;
	return old_handler;
}

void* NewHandlerSupport::operator new(size_t sz)
{
	new_handler global_handler = std::set_new_handler(_current_handler);
	void* mem = nullptr;

	try{
		mem = ::operator new(sz);
	}
	catch(const std::bad_alloc&){
		set_new_handler(global_handler);
		throw;
	}
	set_new_handler(global_handler);
	return mem;
}

void NewHandlerSupport::operator delete(void* p){
	::operator delete(p);
}

void allocate_a_lot(){
	size_t large_memory = 4000000000;
	long* l = new long[large_memory];
}

void show_bad_alloc(){
	try{
		allocate_a_lot();
	}
	catch(bad_alloc&){
		cout << "no more memory: bad_alloc handler" << endl;
	}

	set_new_handler(no_more_memory);
	allocate_a_lot();
}

