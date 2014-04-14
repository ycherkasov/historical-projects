/** @brief 
����� 13 �����������
������� �������. �����������, ���������������, ���������, ���������������, �������� �����
������� �������.
������� �� ������ ���������.
�������������. ������������ � �������. �������������� ��������.
*/

#include <vector>
#include <string>
#include <iostream>

using namespace std;


// ----------------------------------
// ����������� ����� ������� - 
// ����� �������������� ��� ������� ����� 
// ��� �������� ������ ����������
// ������ ����� ����������� ����� �� �������� ������.
template<typename Symbols>
class compare_str{
public:
	virtual void compare(const basic_string<Symbols>& str) = 0;
};

// ----------------------------------
// ����������� ��������� ������
// � ������������ ��������
template<typename Symbols>
class lexigraphical : public compare_str<Symbols>{
public:
	virtual void compare(const basic_string<Symbols>& str);
};

template<typename Symbols>
class no_case : public compare_str<Symbols>{
public:
	virtual void compare(const basic_string<Symbols>& str);
};

// --------------- ���������� ������� --------------- 
template<typename Symbols>
void lexigraphical<Symbols>::compare(const basic_string<Symbols>& str){
	std::cout << "lexigraphical<Symbols>::compare()" << '\n';
}

template< typename Symbols >
void no_case<Symbols>::compare(const basic_string<Symbols> &str){
	std::cout << "no_case<Symbols>::compare()" << '\n';
}
// ----------------------------------


// ----------------------------------
// ��������� �����, � ������� ���������� 2 ���������
// ��� �������� � ��� ���������
// ��������� ����� �������� �� ���������
template<typename Symbols, typename Comparer = lexigraphical>
class container{
	basic_string<Symbols> _str;
	Comparer _c;
public:
	container(basic_string<Symbols>& str);
	void compare(basic_string<Symbols>& str);
};

template<typename Symbols, typename Comparer>
container<Symbols, Comparer>::container(basic_string<Symbols>& str) :
	_str(str)
{}

// ���������� ���������� ��� ������ ��� ��������� �����
template<typename Symbols, typename Comparer>
void container<Symbols, Comparer>::compare(basic_string<Symbols>& str){
	_c.compare(_str);
}

// ������ - ��������� ������������� - 
// ������ ��� �������� ������ ����������
// ��������� ��������� �� vector<void*>
// ����� ���������� ��� ��������� ��� ���������
// ���� ������ �������� ������� vector<T*>
// ������������� - ��� "����������" ��� �������

// ������������ ��������� ������������� �� ������, ����� ��������� ������������ ����
// - ��� ������� ���������� ����������������
template< typename T >
class vector< T* > : public vector< void* >{
public:
	typedef vector<void*>	base;

	vector():base(){}
	explicit vector(size_t i) : base(i){}

	T*& elem(size_t i){ return static_cast<T*&>(base::elem(i)); }
	T& operator[](size_t i){ return static_cast<T*&>(base::operator [](i) ); }
};