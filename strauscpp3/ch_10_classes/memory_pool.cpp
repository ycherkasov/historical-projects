#include <iostream>
#include "memory_pool.h"

using namespace std;


//////////////////////////////////////////////////////////////
// ���������� memory pool � ������



// �����, ���������� ������ ���� ������ memory pool

memory_pool<memory_pool_item> memory_pool_item::pool;

memory_pool_item::memory_pool_item() :i(){
	cout << "memory_pool_item()" << endl;
}

//virtual 
memory_pool_item::~memory_pool_item(){
	cout << "~memory_pool_item()" << endl;
}

//static 
void* memory_pool_item::operator new(size_t s){
	// ������ �������� ������� ������ �� ����
	return pool.alloc(s);
}
//static 
void memory_pool_item::operator delete(void* p, size_t s){
	// ���������� ������ � ���
	pool.free(p, s);
}

void memory_pool_item::test(){
	cout << i++ << endl;
}