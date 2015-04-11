#pragma once
#include <iostream>
using std::cout;
using std::endl;

// ����� 16.2 ����������
// ���������� ������� � ������
// ����������� ������� �������� ������
// ��������� ��� ������� ������

// ������� ������ ������� ������ ����������
// �� ���� ���� ��������
// � ������������ �������
// � � ��� �����, ������������ ������� ��������� �������
// � �������� ����������

//����������� ������ ��� ��������� ����������� ���� ���������� �� ���������� ����� ������ ����.
//��! ����������� ���������� ������ ��� ������, �.�. ��� ��������� ������������.

struct A {};
struct B : A{
	char b; // offsetof(b) = 0, sizeof(B) = 1
};
struct C : A{
	A c; // offsetof(c) = 1, sizeof(C) = 2
};

struct base1{typedef int INT;};
struct base2{};
struct derived : public base1, public base2{
	derived(){};
};

void simple_size(){
	A a = {}; B b; C c;
	cout << "sizeof A " << sizeof(a) << endl;
	cout << "sizeof B " << sizeof(b) << endl;
	cout << "sizeof C " << sizeof(c) << endl; // ���� 1, ���� ������ ���� 2 - �����������

	base1 b1 = {};
	base2 b2 = {};
	derived d;
	cout << "sizeof base1 " << sizeof(a) << endl;
	cout << "sizeof base2 " << sizeof(b) << endl;
	cout << "sizeof derived " << sizeof(c) << endl;
}

// ������������ ���������� �������:
template <typename T1,typename T2>
class my_derived : public T1, public T2{};

// ������������ ������ ��������� � ������������� �������
template <typename Base,typename Member>
class base_memeber_pair : public Base{
private:
	Member _m;
public:
	base_memeber_pair(const Base& b, const Member& m) : Base(b), _m(m){}

	// ������ � ���� � �����
	const Base& first() const {return static_cast<const Base&>(this);}
	Base& first() {return static_cast<Base&>(this);}
	const Member& second() const {return _m;}
	Member& second() {return _m;}
};
