#pragma once

// ������� � ������
// 1.���������� ������������� ������� ����� ���� ������������ ����������� ������� 
// (�.�. ����� ��� � ������ ����� �� �����)
// 2.���������� ������������� ������� �.�. ������������.


// - ���������� ������������� ������� �� ����� ���� ������������.
// - ����� ��������� ������������� ���������� ��������� �������.
// - ������������� ������ ������ ���� ����� � ����� ����������, � ������� �� ������

// ������� ������������� �������:

// ���������� ������� � �������
void comrade(void* ){}

template <typename T>
void comrade(T){}

template <typename T>
class comrade_t;

class comrades{
public:
	// ���������� ����� ��������� � ������������
	friend void comrade(int){}

	// ������������� �������
	friend void comrade(void*);

	// ������������� ��������� ������� (��������� ���������)
	friend void comrade(int);

	// ������������� ��������� ������� (����� �������� ���������)
	friend void comrade<double>(double);

	// ���������� ������������� ��������� ������� ��������� � ������������
	template <typename T>
	friend T comrade(const T t){
		return t;
	}

	// ���������� �������������� ������
	template <typename T>
	friend class comrade_t;
};

// �������� ����������� �������������� ������� � ���������� �������
template <typename T>
class Node;

template <typename T>
class Node2;


template <typename T>
class Tree{
	friend class Node<T>;
	friend class Node2<int>;
};

// ����� - � ��������� ������ ������ ���������� ������������� �������,
// ����� ��� ������� ��������� ODR

template<typename T>
class createor{
public:
	createor(){}

	// ����������������� ��� ������:
	// error C2084: function 'void createor<T>::appear(void)' already has a body
	// friend void appear(){ int i = 1; }

	// ������� ������������� ������� ������������ � ����������� �� ����
	friend void appear( createor<T>* ){ int i = 1; }
};

// ������ ������������ ������������� �������!
void test_friends_odr(){
	createor<int> a;
	createor<double> b;
}