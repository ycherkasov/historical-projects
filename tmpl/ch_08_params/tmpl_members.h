#pragma once
#include <iostream>
using std::cout;
using std::endl;
// --- ������������ ������ ���������� ������ --- 
// 1.�������-�����
// 2.��������� ������
// 3.����������� �����

template <typename T>
class tmpl_members
{
public:
	tmpl_members(const T& v);

	// ������� �����
	void open();
	
	// ��������� �����
	template <typename T2> 
	void copy(const T2& t);
	
	// ����������� ����
	static double s_dbl;

	// ��������� �����
	class self{
	public:
		self(int i);
		int _i;
	} _self;

	// ��������� ������
	template <typename T3>
	class self_tmpl{
	public:
		self_tmpl(const T3& t);
		T _t;
	};
	
	self_tmpl<T> _self_tmpl;
private:
	T _val;
};

// ����������� ����������� (�������� ������������ ��������� �������)
template <typename T>
tmpl_members<T>::tmpl_members(const T& v) : _self(5), _self_tmpl(v), _val(v){}

// ����������� �������� ������
template <typename T>
void tmpl_members<T>::open(){

}

// ����������� ���������� ������
template <typename T>
template <typename T2>
void tmpl_members<T>::copy(const T2& t){

}

// ����������� ������������ �����
template <typename T>
double tmpl_members<T>::s_dbl = 0.;

// ����������� ������ (������������) ���������� ������
template <typename T>
tmpl_members<T>::self::self(int i) : _i(i){}

// ����������� ������ ���������� �������
template <typename T>
template <typename T3>
tmpl_members<T>::self_tmpl<T3>::self_tmpl(const T3 &t) : _t(t){}

// --- ������������ ����������� ������� � �������� --- 
template <typename T>
class base {
public:
	// ������� ����� ����� ���� ����������� � �������
	virtual void open(const T& t){
		cout << "base::open " << typeid(t).name() << endl;		
	}
	// ...� ��������� - ���
};

template <typename T>
class derived : public base<T> {
public:
	// ����� ��������� ���������� ���� ��� base<double>
	virtual void open(const T& t){
		cout << "derived::open "<< typeid(t).name() << endl;		
	}
};

// ------------------- ���������� �������� -------------------

// � ������� �� �������, ��� �������� �� ����������� ���������� ���� � ������� ���������
// ������� �� ����� ����� ���������� �� ������ extern "C", �� ����� ����� 
// ������������� ���������� �� ������ extern "X"

// ������� ����� ������� ����������, ����� ����������� ��� static
template <typename T>
void external();	// �������

template <typename T>
static void internal();	// ����������


