#pragma once
#include <iostream>
using std::cout;
using std::endl;

//  ����� 16.4 ����������
// ����������������� �������������

// �������������� ������������� �������
class non_virtual{};

class my_virtual{
public:
	virtual void foo(){}
};

template <typename VBase>
class Base : private VBase{
public:
	// ������������� foo() ������� �� ����, ����� ����� ������� ����������
	// ��� ������� - � �������������� ��� ���
	// ����������� ����� �������� - ���������� �� �����
	void foo(){ cout << "Base::foo"  << endl; }
};

template <typename IsVirtual>
class Derived : public Base<IsVirtual>{
public:
	// ������������� foo() ������� �� ����, ����� ����� ������� ����������
	// ��� ������� - � �������������� ��� ���
	void foo(){ cout << "Derived::foo"  << endl; }
};
