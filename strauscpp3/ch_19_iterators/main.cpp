#include <iostream>
#include <list>
#include <vector>
#include "checked_iterator.h"
#include "custom_allocator.h"

using namespace std;

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

void show_custom_allocator(){

}

int main(){

	show_iterator_traits();
	show_checked_iterator();
	show_custom_allocator();

	return 0;
}