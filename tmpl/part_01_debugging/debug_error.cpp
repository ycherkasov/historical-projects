#include <list>
#include <string>
#include <algorithm>

using namespace std;

// ------------------- ���������� ���������� ��������� -------------------
void show_long_error(){
	list<string> coll;

	// ����� �������� ������
	// � �������� greater ������� �������� ������� int
	// ����� ��������� ���������� �� std::string
	list<string>::iterator pos =
		find_if( coll.begin(), coll.end(), 
		bind2nd( greater</*int - ������!*/string>(), "A" ) );
	/*
	error C2440: 'initializing' : cannot convert from 'const char [2]' to 'int'
	There is no context in which this conversion is possible
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(15) : 
	see reference to function template instantiation 'std::binder2nd<_Fn2> 
	std::bind2nd<std::greater<_Ty>,const char[2]>(const _Fn2 &,const char (&))' being compiled
	with
	[
	_Fn2=std::greater<int>,
	_Ty=int
	]
	*/

	// ��� �����, ���������� ��������� ���������� ����������
}

// ------------------- ������ ��������������� -------------------
// ���������� �������� ���������� ��������� �������

// ��� ������� ������������, ��� T - ���������!
template<typename T>
void clear(T& p){
	*p = 0; // - ������!

}

// ��� ������ �������� � ��� �������� (�� ����������� ���������!)
template<typename T>
void core(T& p){
	clear(p);
}

// ��� ����
template<typename T>
void middle(T& p){
	core(p);
}

// ������� ������� � ��������, �������� �������� ���� int
// ��� ��� "�� ���������"
template<typename T>
void up(const T& env){
	// ���-�� � env
	typename T::index_t p;
	middle(p);
}

class client{
public:
	typedef int index_t;
};

void test_stack_error(){
	//client cli;	// - ����������������� ��� ������ 1
	//up(cli);		// - ����������������� ��� ������ 1
	// ��������� �� ������� �� ���� �������� � ����� ������
	/*
	error C2100: illegal indirection
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(45) : 
	see reference to function template instantiation 'void clear<T>(T &)' being compiled
	with
	[
	T=client::index_t
	]
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(51) : 
	see reference to function template instantiation 'void core<T>(T &)' being compiled
	with
	[
	T=client::index_t
	]
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(60) : 
	see reference to function template instantiation 'void middle<client::index_t>(T &)' being compiled
	with
	[
	T=client::index_t
	]
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(70) : 
	see reference to function template instantiation 'void up<client>(const T &)' being compiled
	with
	[
	T=client
	]
	*/
}

// ��� �������� �������� �������� ������������ ������ "������� ���������������" - 
// �� ������� ������ ����������� �������� ���������� ��������� �� ������������ ��������� �����������
// � ������ ������ �� ������������� ���������. �������� ���:
template <typename T>
class shallow_checks{
public:
	// ������ �������, ����������� ����������� -
	// ����� �������� �� ������������
	static void ignore(const T& p){ T a = p; }
	static void check_is_pointer(T ptr){
		ignore(*ptr);
	}
};

template<typename T>
void up1(const T& env){
	// ���-�� � env
	
	// �������������� index_t �����
	typename T::index_t p();
	
	// ��������� �������� �� ���������:
	// shallow_checks<T::index_t>::check_is_pointer(p);// - ����������������� ��� ������ 2
	// error C2100: illegal indirection
	// middle(p);									// - ����������������� ��� ������ 2
}

void test_shallow_check(){
	client cli;
	up1(cli);
}