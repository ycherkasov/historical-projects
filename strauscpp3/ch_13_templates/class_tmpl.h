#include <iostream>
using std::cout;
using std::endl;
// ��������� ��������� �������

// ------------------- �������, ������ -------------------
// ������ ������� � ��������� ������
// ���������� �� ������� � ������� ����������
// ������, ����� ������ ������ �� ���������,
// �� � ������ ������ ��� ����� bad design
template <typename T, int MAX>
class fixed_array{
public:
	// ������� �������� ������������� ������ ��������� ��������� typedef
	typedef T			value_type;
	typedef T&			reference;
	typedef const T&	const_reference;

	fixed_array();
	T& operator[](int index);
	const T& operator[](int index) const;

	// ������� ����� ���� �������� � � �������� ����� ���� ������
	// - ����������� ������������� �������� ��� �� �������� �������� - 
	// �� ������ ������� �� ��� ���������. �� ������ �� ���� ��������,
	// ����� ����� �������� ODR
	// - ������������� ������� ������� ����� ���������� � ����� �������
	// (��� �������� ��� �������� ���������� �����)
	friend std::ostream& operator<< <>(std::ostream& os, const fixed_array<T,MAX>& f){
		for(int i = 0 ; i < MAX; i++){
			os << _arr[i];
		}
		return os;
	}

private:
	T _arr[MAX];
};

template <typename T, int MAX>
fixed_array<T,MAX>::fixed_array() : _arr(){}

template <typename T, int MAX>
T& fixed_array<T,MAX>::operator [](int index) {
	if( (index < 0) || (index >= MAX) ) 
		throw std::out_of_range("Array ranges error");
	return _arr[index];
}

template <typename T, int MAX>
const T& fixed_array<T,MAX>::operator [](int index) const {
	if( (index < 0) || (index >= MAX) ) 
		throw std::out_of_range("Array ranges error");
	return _arr[index];
}

// �������� ��������� ����� ������������ � � ��������
// ������, ������� ����������, ��
template <typename T, int VAL>
T add_value(const T& t){
	return t + VAL;
}

// ��������� ��������� �������� ����� ����
// - �������������� �����������
// - ����������� �� ������� � ������� �����������

// �������� ������� ����� ���� ������������ ��� ������������ 
// ����������� ������ ����������
template <typename T, T def_val> class cont;

// ------------------- ��������� ������� -------------------
// �������� ��������� ��������:
// - ����-������ � ������ (�.�. ��������������) ��� �������
// - �����-������, � �.�. ����������� � ������ ���� �������
// - ��������� ������ � ������ ��� �������
// - ��������� ����� � ������ ��� �������

// ������ �� ����� ���� ��������� ������ �������!
// ����� ������ ���������� ���������� ���������� ���������� ������
// � ������������ - ��������� ��� ��� ��������������


// 1.�����
class no_templ{
public:
	// ���� � ������
	fixed_array<int, 100> _array;	
	no_templ() : _intern(5){}

	// ��������� ����������� � ������
	// ��������� ������������ � ��������� ����������� 
	// ������� �� �������� ������������� � ���������� �� ���������
	template <typename T>
	no_templ(const T& t): _intern(5){}

	// ��������� � ����� ��������� ����� 
	template <typename T2>
	struct internal_no_templ{
		internal_no_templ(const T2& t) : _param(t){}
		T2 _param;
	} /*_internal - ������ ���������� ������ � ������ -
	  ��������� ��� �������������� */;

	internal_no_templ<int> _intern;
};

// 2.������
template<typename T>
class templ{
public:
	// ���� � �������
	fixed_array<T, 100> _array;

	// ��������� ����������� � �������
	templ(const T& t) : _intern(t){}

	// ��������� � ������ ��������� ����� 
	template <typename T2>
	struct internal_no_templ{
		internal_no_templ(const T2& t) : _param(t){}
		T2 _param;
	} /*_internal - ������ ���������� ������ � ������ */;

	internal_no_templ<T> _intern;
};

// ------------------- ������������ -------------------
// ������ �� ������
// ����� �� �������
// ������ �� �������

// 1.������ �� ������
// ����������� ������� � ������� ������
template<typename T>
struct common_counter{
	common_counter(){++count;}
	~common_counter(){--count;}
	static unsigned long count;
};

template<typename T>
unsigned long common_counter<T>::count = 0;


class Xclass : public common_counter<Xclass>{
public:
	Xclass(){}
};

class Yclass : public common_counter<Yclass>{
public:
	Yclass(){}
};
// 2. ����� �� ������� - ��. CRTP.h � ������� � ����� ����������

// 3.������ �� ������� - ��. ������ � tmpl.h

// ------------------- ������������ �� �������� � �������������� -------------------
// � ����� ������ �++ ��� ������, ��������������� �� ������ �������,
// �� ������� �������� ����������� ������������. �������:

class Shape{};

class Circle : public Shape{};

template <typename T>
class Set{};

void get_shape(Set<Shape*>& s){

}

void get_circle(Set<Circle*>& c){
	// ������ ��������������
	//get_shape(c); - ����������������� ��� ������
}