#include <iostream>
#include <list>
#include <set>
#include <vector>
#include <cstdlib>
#include "checked_iterator.h"
#include "custom_allocator.h"

using namespace std;

// � C++ ��������� ����������� ���������� �������
// ������� ��������� ��������� � �������������� �������� ����� �������������� ������
// ��������, sort(++a, b) - ���� a - ���������, �� ��������������, ���� ������ - ��������


void show_iterator_traits(){
	// �������� - ��� ���������� (��, ��� ����� ���� ��� �������� - ��� ��������)
	// ���� ����� ��� ��������� ��� ������

	// ���� ����������:
	// Output
	// Input
	// Forward
	// Bidirectional
	// Random-access
	int a[] = {0,1,9,3,5,4,2,7,6,5};

	// ������ ����� Random Access ���������
	vector<int> v1(a, a + sizeof(a)/sizeof(int));

	// � ������ ����� Bidirectional
	list<int> l1(a, a + sizeof(a)/sizeof(int));

	// ���������� ����� �����������
	// ��� �������
	typedef vector<int>::const_iterator::difference_type vect_int_dist;
	vect_int_dist vdist = distance(v1.begin(), v1.end());

	// ����� �������� �� ��������� (������ � ���������)
	vector<int>::iterator lst = v1.end() - 1;
	iter_swap(v1.begin(), lst);

	// ��� ������
	typedef list<int>::const_iterator::difference_type list_int_dist;
	list_int_dist ldist = distance(l1.begin(), l1.end());

	// � �������� ���������� ����� ������ ���������� ������ "��������� ���������"
	// �� ��������� ������� ����������, ��������������� ���� ���������
	// ��������, ��� RA ��� ������ ��������,
	// � ��� BI ��� �������� �� ������ ��������� �� �������

	// ���������� � ��������� advance (������ +=)
	// += 5 (�� �� ��� � operator[])
	advance(v1.begin(), 5);

	// �������� �� ������ (������ operator[] ��� std::list)
	advance(l1.begin(), 5);

	// �������� ����� ���������-�������
	// � �������� ��������� (����������� � 18)
	// � ����� ��������� ������� (����������� � 03)
	// � �� ������� �����, ��������, ��������� ������ ������, ������ ������ ������ ������ � ��
}

void show_checked_iterator(){

	// ���������� � ��������� 
	Checked_container< vector<int> > v1;
	Checked_container< vector<int> > v2(10);

	Checked_container< list<int> > l1;
	Checked_container< list<int> > l2(10);

	// ��������� � ��������� 
	try{
		// ������������� v2.begin()
		Checked_container< vector<int> >::iterator it(v2);
		while(true) it++;
	}
	catch(const std::exception& e){
		cout << "exception: " << e.what() << endl;
	}

}

void show_set_iterator(){
	set<int> s;

	// ��������� ������
	associative_insert_iterator< set<int> > aint(s);
	
	*aint = 1;
	++aint;
	*aint = 2;
	++aint;
	*aint = 3;
	++aint;

	// ������� ������
	associative_inserter(s) = 4;
	associative_inserter(s) = 5;

	int vals[] = {6,7,8,9};
	copy(vals, vals + sizeof(vals) / sizeof(int), associative_inserter(s));
}


void show_custom_allocator(){
	// ������� � ������� ���������������� � ����� ������ �� ������ �����������
	// ��������, list::splice � ������� ���������������� ����� ��������
	// �� �� �������� �������, � ������������ ��������

	typedef std::vector<int, std::custom_allocator<int> > custom_vector;
	custom_vector v;
	v.push_back(1);
	v.push_back(2);
	v.pop_back();

	// � STL ������ ������� ��� �������������� ������ � 
	// ����������������� ������������ � �����������
	// ��� ���������� ������� �������� ����������

	// ������������� ������ ���������
	int arr[10] = {};
	uninitialized_fill(arr, arr + sizeof(arr) / sizeof(int), 1);

	// ������������� n ��������� ���������
	uninitialized_fill_n(arr, 5, 2);

	// ����������� ��������� � ������
	int arr2[10] = {};
	uninitialized_copy(arr, arr + sizeof(arr) / sizeof(int), arr2);

	// ���������� ��������� ��� �������� �������������������� ������
	// raw_storage_iterator � �� ������������� T()

	// ��� �������������� ������������� ������ �����������:
	pair<int*, ptrdiff_t>p = get_temporary_buffer<int>(10);
	// ���������� ���� - ��������� �� ������ � ������

	// � ����������� ��� �������� �� 0
	if (p.first != 0)
		return_temporary_buffer(p.first);
}

void show_c_memory(){

	// ������� C ��� ���������� "�����" �������

	// ������� 100 ����
	void* m = malloc(100);

	// �������� � �������� ������ ��� 100 int
	void* c = calloc(100, sizeof(int));

	// �������� ������ �� 200
	void* m1 = realloc(m, 200);
	if (m1 != NULL) {
		m = m1;
	}
	else {
		free(m);
		puts("Error (re)allocating memory");
	}
	free(m);
	free(c);
	// ����� ���� 
	// memcpy() - ����������� ��� ����������
	// memmove() - ����������� � �����������
	// memchr() - ����� ���� �� ��������
	// memcmp(), memset()
}

int main(){

	show_iterator_traits();
	show_checked_iterator();
	show_set_iterator();
	show_custom_allocator();
	show_c_memory();

	return 0;
}