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
#include <algorithm>

using namespace std;


// ----------------------------------
// ����������� ����� ������� - 
// ����� �������������� ��� ������� ����� 
// ��� �������� ������ ����������
// ������ ����� ����������� ����� �� �������� ������.
template<typename Symbols>
class compare_str{
public:
	virtual bool compare(const basic_string<Symbols>& str1, const basic_string<Symbols>& str2) = 0;
};

// ----------------------------------
// ����������� ��������� ������
// � ������������ ��������
template<typename Symbols>
class lexigraphical : public compare_str<Symbols>{
public:
	virtual bool compare(const basic_string<Symbols>& str1, const basic_string<Symbols>& str2);
};

template<typename Symbols>
class no_case : public compare_str<Symbols>{
public:
	virtual bool compare(const basic_string<Symbols>& str1, const basic_string<Symbols>& str2);
};

// --------------- ���������� ������� --------------- 
template<typename Symbols>
bool lexigraphical<Symbols>::compare(const basic_string<Symbols>& str1, const basic_string<Symbols>& str2){
	return str1 > str2;
}

template< typename Symbols >
bool no_case<Symbols>::compare(const basic_string<Symbols> &str1, const basic_string<Symbols>& str2){
	std::string nocase1, nocase2;
	std::transform(str1.begin(), str1.end(), std::back_inserter(nocase1), ::tolower);
	std::transform(str2.begin(), str2.end(), std::back_inserter(nocase2), ::tolower);
	return str1 > str2;
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
	_c.compare(str, _str);
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