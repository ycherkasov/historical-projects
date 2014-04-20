#include "user_allooc.h"
#include "construct.h"

#include <cstdio>
#include <iostream>

using namespace std;


user_alloc::user_alloc(void) :_test(0) {
	
	// representation in memory
	// vptr			test int	
	// 20 ea 87 00	00 00 00 00	
	cout << "user_alloc()" << endl;
}

user_alloc::~user_alloc(void) {
	cout << "~user_alloc()" << endl;
}

void user_alloc::test(){
	cout << "test = " << _test << endl;
}

void* user_alloc::universal_allocate(size_t s, new_handler handler/* = 0*/) {
	
	void* p = 0;
	// ��� ���������� ��������� new ���� �������������� ���������� ������� ������
	// 1. �������� �� 0
	// ����������� ���� - ��������� 0 �� 1 ��� �����
	if(0 == s){
		s = 1;
	}

	new_handler oldHandler = 0;
	if (handler)
		oldHandler = std::set_new_handler(handler);

	// 2. ��������� �������� �������� ������
	while(true){
		p = malloc(s);
		if(p){
			break;
		}
		else{
			// ���� � ���������� �������� globalHandler
			// ��������� � 0 � ������ ������ �������
			new_handler globalHandler = set_new_handler(0);
			set_new_handler(globalHandler);
			if(0 == globalHandler){
				// �� �������� ����������, ������ ������ ����������
				throw bad_alloc();
			}
			else{
				// return global handler as new_handle()
				set_new_handler(globalHandler);

				// call it
				globalHandler();
			}

		}
	}
	if (handler)
		std::set_new_handler(oldHandler);
	return p;
}

void user_alloc::universal_free(void* p){
	// ������ 1 ������� 8
	// ��� operator delete() ���������� ������������� �������� �� ����
	if(0 == p)
		return;
	free(p);
}

void* user_alloc::operator new(size_t s) {
	return universal_allocate(s);
}

void user_alloc::operator delete(void* p){
	universal_free(p);
}


void* user_alloc::operator new[](size_t s) {
	return universal_allocate(s);
}

void user_alloc::operator delete[](void* p){
	universal_free(p);
}


// ������������� ����� ����������� ��������� new
void* user_alloc::operator new(size_t s, void* place){
	printf("placement new\n");
	return place;
}

// ������������� ����� ����������� ��������� delete
// (����� ���������� ������ � �������������� �����)
void user_alloc::operator delete(void* p, void* place){
	// do nothing
	printf("placement delete\n");
}

// 4. ������������� ����� ��������� new, ����������� new_handler
void* user_alloc::operator new(size_t s, new_handler p){
	return universal_allocate(s, p);
}

void user_alloc::operator delete(void* p, new_handler h){
	universal_free(p);
}

//If class-level operator delete is a template function, 
// it must have the return type of void, the first argument void*, 
// and it must have two or more parameters. 
// In other words, only placement forms can be templates.

