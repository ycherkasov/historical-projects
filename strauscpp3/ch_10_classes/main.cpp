#include "construct.h"
#include "class.h"
#include "user_allooc.h"
#include "word_counter.h"
#include "new_forms.h"
#include "memory_pool.h"
#include "ref_count.h"

#include <iostream>

using namespace std;

construct g_construct;


// ����������� ���������� ��������� � �� ��������� ������ ����� 
// (�� �� ��� class, enum, union)
void struct_one_name(){
	struct A{
		int a;
	};

	A aa;
	aa.a = 2;
	int A = 1;
}

// ������� ������������� ������������� � ������ 
// � ������������ ��������� � mutable-�������
void show_const(){

	A a;

	bool b = false;
	size_t s = sizeof(b); // 1

	const A aa;
	//aa.set_int1(5); - ��� ������, ����� �������������� ������ � ����������� ������

	// � ��� �����, ���� ������������ ������ mutable ����� ���� �������
	aa.set_a6_mutable(5);

}

// ������� ������������� ������������� � ������ 
// � ������������ ����������� � ����������� �� ���������
void show_const_pointers(){

	// ��������� �� ���������
	const int* pp1 = new const int(3);

	// ����� ������ - ��������� �� �������������������,
	// �� ���������� ������
	const int* pp2 = new const int;

	// ������������������� �� ��������� �������� ������ �� �������
	// �� ��������� ���� const_cast
	//const_cast<int>(*pp2) = 1;

	// ����� ������� ����������������������� ����������� ������������ ������
	// �� ���� �� ����� �������, � ������ � ��� ������ ������� ������.
	const int* pp3 = new const int[100];

	// �������� ��������� ������������ ������ �� ������������ ���������
	int* const pp4 = new int[100];

	// �������� - �������� ��������� ������������ ������ �������� �������!
	int* pp5 = new int[0];
	// � ����������� - �� ��������� (���� ���� ���������� gcc, ��� ��� �����)
	//int p6[0];

	// ������� _msize ��������� ������ �������� ������ ����������� �������.
	size_t sz = _msize(pp4);
	sz = _msize(pp5);

	delete pp1;
	delete pp2;
	delete[] pp3;
	delete[] pp4;
	delete[] pp5;

	// ��������� �������� typedef � ��������
	typedef good_weekday_t week_array[7];
	good_weekday_t* s = new week_array;
	//delete s; - �����������! ����� delete[], �� �� �� ���� �� �����!
	delete[] s;

	//X* x = new Y[2];
	// delete[] x; 
	//It invokes undefined behavior.
	//This is correct.  
	//When deleting an array, the dynamic and the static type of the object must be the same, 
	//or the behavior is undefined (C++ Standard 5.3.5/3).
}

// ������� ������ ������������ �������:
// 1. ����������� �������������� ������(�� ���������, �����).
// 2. ������ � ��������� ������ (new/delete).
// 3. ������������� ������-���� ������.
// 4. ������ - ������� �������.
// 5. ��������� ����������� ������.
// 6. ���������� ������.
// 7. ��������� ������.
// 8. ������, ����������������� ���������������� �����������.
// 9. ���� ����������� (union)
void show_construct_destruct(){

	std::cout << std::endl <<  "Create automatic objects" << std::endl;
	// 1. ����������� �������������� ������(�� ���������, �����).
	// ���� � ������� ���� ��������� ��� ������, �� �� ����� ���� ������ 
	// ������������� �� ���������
	{
		construct c;
		construct d;
		{
			construct c1(c);
		}
		noncopyble nc;
		// � ����������� ���� ������ ������!
		// noncopyble nc1(nc);
	}
	// ��������� � ������� c-d-c1-nc, ������������ c1-nc-d-c
	

	// 2. ������ � ��������� ������ (new/delete).
	std::cout << std::endl << "Create dynamic objects" << std::endl;

	// ���� � ������ ������ ������������� ��������������� �����������,
	// �� ��� ����� ������, � (), ������� ������...
	construct* pc1 = new construct();
	// ... � ��� - ���
	construct* pc2 = new construct;

	// ��������� �������������� ���������!
	delete pc1; pc1 = 0;
	delete pc2; pc2 = 0;

	// 3. ������������� ������-���� ������.
	std::cout << std::endl << "Create non-static member" << std::endl;
	// ��������� ����������� �� ��������� intern()
	{
		construct c1;
	}
	

	// 4. ������ - ������� �������.
	std::cout << std::endl << "Create objects array " << std::endl;
	// ! �������� ������� ����� ���� ��������������� �� ���������
	construct arr_c[5];
	// ����� ������������ ������ ������������� � ���� �������������
	construct arr_1[] = { construct(2), construct( g_construct ) };
	// ����� ������������ ���� ������� �������������� ��� ���������������
	construct arr_2[] = {1, 2};
	// ����.-����� ������� ����� ��������� ��� �������� ���������� ��������� - []

	// 5. ��������� ����������� ������.
	std::cout << std::endl << "Create local static objects " << std::endl;
	create_static(0);
	create_static(1);
	create_static(2);

	// � ������ ������� �������� ����� ������������ �������������� � ���� �������
	int st = return_static();

	// 6. ���������� ������.
	// ������������ ���������� �������� ���������� � ����� ������ - � ������� �����������
	// � ������ ������� - ������� �� ���������. �� �� ��� ���������� �������� (��. g_construct, g_intern)
	// ������� ������������ ��������, � ����� �������-������������� (��. construct.h)

	// 7. ��������� ������.
	// ������� �����
	std::cout << std::endl << "Create temporary object " << std::endl;
	construct(5);
	// ����� ������� ����������� ���������� ��� �������� � ����������� �� �������� 
	// � �������� ����������

	// 8. ������, ����������������� ���������������� �����������.
	{
		// ��. show_user_alloc()
	}
	

	// 9. ���� ����������� (union)
	// union �� ����� ����� ������ � ��������������-�������������
	// ������ ��� ��������� ����� �� ��� ��������
}

// Task from B.Batkin
void show_word_counter(){
	string s( "test.txt" );
	word_counter::count_from_file( s );
}



void show_placement_delete(){
	
	struct X {
		X() {
			throw std::runtime_error("X object never be created");
		}
		// custom placement new
		static void* operator new(std::size_t sz, bool b){
			std::cout << "custom placement new called, b = " << b << '\n';
			return ::operator new(sz);
		}
		// custom placement delete
		// could be called in functional form only
		// however, pair delete could be found in case of exception
		static void operator delete(void* ptr, bool b)
		{
			std::cout << "custom placement delete called, b = " << b << '\n';
			::operator delete(ptr);
		}
	};

	try {
		X* p1 = new (true) X;
	}
	catch (const std::exception& e) {
		cout << e.what();
	}
}

void test_placement_new_handler(){
	printf("No more memory!\n");
	throw bad_alloc();

}

void show_user_alloc(){

	// ������ ������ ������������� new() � delete()

	// simple new overload
	{
		cout << "Test simple new overload" << endl;
		user_alloc* x = new user_alloc();
		x->test();
		delete x;
	}

	// new for array overload
	{
		cout << "Test array new overload" << endl;
		user_alloc* x = new user_alloc[10];
		x[1].test();
		delete[] x;
}

	// placement new overload
	{
		cout << "Test placement new overload" << endl;
		user_alloc* x = new user_alloc();
		x->test();
		x->~user_alloc();

		user_alloc* y = new(x)user_alloc();
		// could be directly called in functional form only!
		// will be implecetly called in case of exception
		user_alloc::operator delete(y, x);
		delete x;
	}

	// new_handler replacement new overload
	{
		cout << "Test placement new overload" << endl;
		user_alloc* x = new (test_placement_new_handler)user_alloc();
		x->test();
		delete x;
	}
}

void show_new_handler(){
	
	// ���������������, ��� ������������ mixture-������� �� ������ 1-7,
	// ���������� ���������� new_handler �����

	// ��������� � ����� ������� ��������
	try{
		LargeObject::set_new_handler(NewHandlerSupport<LargeObject>::no_more_memory);
		LargeObject* x = new LargeObject();
		delete x;
	}
	catch (const std::bad_alloc& e){
		cerr << "Lack of memory: " << e.what() << '\n';
	}
	
	// ��������� � ������� ��������
	try{
		SmallObject::set_new_handler(NewHandlerSupport<SmallObject>::no_more_memory);
		SmallObject* x = new SmallObject();
		delete x;
	}
	catch (const std::bad_alloc& e){
		cerr << "Lack of memory: " << e.what() << '\n';
	}

}

void show_memory_pool(){

	// allocated from pool, addr = 0x003789d0
	memory_pool_item* mpi1 = new memory_pool_item();
	mpi1->test();

	// allocated from pool, addr = 0x003789d8 (prev + sizeof memory_pool_item)
	memory_pool_item* mpi2 = new memory_pool_item();
	mpi2->test();

	// deallocated both from pool
	delete mpi1;
	delete mpi2;

	// allocated from pool again, should again addr = 0x003789d0
	// (block 0 free again)
	memory_pool_item* mpi3 = new memory_pool_item();
	mpi3->test();
	delete mpi3;

}

//delete ������ delete[] ��������� � �������, ���� � ���� ���� ������������� ����������(������ ? ).
class class_with_non_trivial_destructor
{
private:
	size_t s;
	int * data;
public:
	class_with_non_trivial_destructor(size_t new_s = 10)
		:
		s(new_s)
	{
		printf("class_with_non_trivial_destructor\n");
		data = new int[s];
	}

	~class_with_non_trivial_destructor()
	{
		printf("~class_with_non_trivial_destructor\n");
		if (data) delete[] data;
	}
private:
	class_with_non_trivial_destructor(const class_with_non_trivial_destructor&);
	class_with_non_trivial_destructor &
		operator = (const class_with_non_trivial_destructor&);
};

//=============================================================================
void crash()
{
	class_with_non_trivial_destructor * bad_array =
		new class_with_non_trivial_destructor[10];

	// � ������ ����������� ������ _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));
	// � ������ ������
	delete bad_array;
}
//=============================================================================
// �� ������ new[] - � ������, ���� ���������� �������������, �� ���� � delete[] �����, 
// � �������� �������� ������� ����������.��� �� ������� ��� ����������, 
// ���� delete �������� �� ���� ������ ���������, �� �������� ����� �� ������� �������� ? 
// ����������� ������ ������ ��� ���������� � 4 - � ������ ����� ��������
void array_size_info()
{
	class_with_non_trivial_destructor *pa = new class_with_non_trivial_destructor[100];
	int *pi = reinterpret_cast<int*>(pa);
	std::cout << pi[-1];
	delete[] pa;
}
// delete, � ������� �� delete[], �� ����� �� ���� 4-�������� ��������, 
// 
// ��, � new / delete, � �������������� �������� ������(�� ��������� - malloc / free) -
// ��� ��� ����, ������� ����� �� ������� ����� �� �����.
// + 4 ����� ��������� ����������, � �� malloc / free, ������� ���������� 
// � �� ����(����������)� � memory manager ��� ������������ ������������ �� ��� ���������, 
// ������� �� �����, � ��������� �� 4 - �� �����

// ������ �������: ����� � ������ delete �� ������ �������� � ������� ��-�� �������� ������������ �����������?


void show_ref_count(){
	rc_string s1("Skotobaza");
	rc_string s2(s1);
	const rc_string s3 = s1;
	s2 = s3;

	const char c1 = s1[0];
	const char c2 = s3[0];
	char c3 = s2[0];
}

int main(){
	
	show_ref_count();
	return 0;

	// delete/delete[] crash
	//crash();
	//return 0;
	array_size_info();

	struct_one_name();
	
	// should be reproduced in release
	//show_bad_alloc();
	//return 0;

	

	show_const_pointers();

	A a(1,1,1);
	int aa = a.get_int6();
	show_const();
	show_construct_destruct();

	show_user_alloc();
	show_new_handler();
	show_placement_delete();
	show_memory_pool();
	
	// ����������� ����� ����� �������� ��� ��� ������, ��� � ��� �������
	construct::out_static_array();
	construct c;
	c.out_static_array();

	// ������������ ����������� ������������� ���������� new
	show_new1();
	show_new2();
	show_new_delete();


	return 0;
}
