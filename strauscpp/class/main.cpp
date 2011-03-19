#include "construct.h"
#include "class.h"
#include "user_allooc.h"
#include "intset_t.h"
#include "word_counter.h"

#include <iostream>

construct g_construct;

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
		void* buf = reinterpret_cast<void*>(0xF00F);

		// ������������� ����������� ��������� new
		// ������ ����� ������ �� ������ buf
		user_allooc* ua = new(buf)user_allooc;
		delete ua;
	}
	

	// 9. ���� ����������� (union)
	// union �� ����� ����� ������ � ��������������-�������������
	// ������ ��� ��������� ����� �� ��� ��������
}

void bool_logics(){

	// ������������� ���������� � ����������� ������� �������� �� ���������
	std::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);

	std::vector<int> v2;
	v2.push_back(3);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);

	intset_t set1(v1);
	intset_t set2(v2);

	intset_t set_sum = set1 + set2;
	set_sum = set1 - set2;
	set_sum = set1 * set2;
}

int main(){

	word_counter w;
	std::string s( "test.txt" );
	w.count_from_file( s );

	show_const_pointers();

	A a(1,1,1);
	int aa = a.get_int6();
	show_const();
	show_construct_destruct();
	
	// ����������� ����� ����� �������� ��� ��� ������, ��� � ��� �������
	construct::out_static_array();
	construct c;
	c.out_static_array();

	// ������������ ������������� ���������� new
	show_new1();
	show_new2();
	show_new_delete();
	bool_logics();


	return 0;
}
