#include "construct.h"
// ������������ ������� �� �������1 ��.4
#include "class.h"

#include <iostream>

// ------------------------------ 
// intern - ������������
intern::intern() : a(){
	std::cout << "intern() default constructor" << std::endl;
}

intern::intern(int i) : a(i){
	std::cout << "intern(int) constructor" << std::endl;
}

// intern - ����������
intern::~intern(){
	std::cout << "intern destructor" << std::endl;
}

// ------------------------------ 
// construct - ������������

// ������� �������������� ����������� �������
const char construct::ch_array[] = "qwertyuiop";
const int construct::int_array[] = {0,1,2,3};
int construct::_counter = 0;

construct::construct() : arr(){
	std::cout << "construct() default constructor" << std::endl;
	_counter++;	// ��������� ������� ��������
}

// ������� ��������� ����������� intern()
construct::construct(int i) : intrn(i), arr(){
	std::cout << "construct(int) constructor" << std::endl;
	_counter++;
}

construct::construct(const construct& c) : intrn(c.intrn.a), arr(){
	std::cout << "construct copy constructor" << std::endl;
	_counter++;
}

// construct - ����������
construct::~construct(void){
	std::cout << "construct destructor" << std::endl;
	_counter--;	// ��������� ������� ��������
}

void construct::out_static_array(){
	// �� ������������ ������� ����� �������� ������ ����������� �����
	// ��� ����� �������� ��� ��� ���������� ������, ��� � ����������, ��� ������������ ����
	// construct::out_static_array() � c.construct::out_static_array();
	std::cout << ch_array << std::endl;
}

// ��� ������������ �� �������� ��������� ��������� �����
// forward declaration
A construct::get_a(A a){
    A a1(a);
    a1.set_a5(5);
    return a1;
}

void create_static(int i)
{
	static construct stat_c1;
	if(i){
		static construct stat_c2;
	}
}

int& return_static(){
	static int res = 0;
	return res;
}

