#include <deque>
#include <cstring>
#include <complex>

#include "func_tmpl.h"
#include "class_tmpl.h"
#include "non_type_params.h"
#include "basics_tmpl.h"

using std::string;

// ������������ �������� ������� �������
void show_func_template(){
	
	int i = 10;

	// ���������� ����� ����������� namespace - ::max
	// ����� �� ���������� � std::max
	::max(7, i);
	::max(3.4, 7.6);

	std::string s1("atatat");
	std::string s2("bdbdbdb");
	::max(s1, s2);

	// �� ���� ���� ����������� ������ ����������� ���� ��������� �������������
	// ����������� �.�. "����� ���������� ������" - ����� ���������� ������� � ��� ����������
	// ����� ��� ������ ����
	int a = ::max<int>(i, 5);

	// ��� ����������� ��������������� ��� ���� T ������ ���� ���������� �������� ���������
	// �������� ��� std::complex �������� �� �����
	std::complex<double> c1(0.1,0.2);
	std::complex<double> c2(.0,0.5);
	// ::max(c1, c2);
	// error C2784: 'bool std::operator >(const std::basic_string<_Elem,_Traits,_Alloc> &,const _Elem *)' 
	// : could not deduce template argument for 'const std::basic_string<_Elem,_Traits,_Alloc> &' 
	// from 'const std::complex<double>'

	// �.�. ����������, ����� ������ �������� ��� �������� ������� ���� ���������

	// ����� ������� ��� ������� ������������� 2 ���� - 
	// ��� �������� �������������� ������������ � ��� ���������������

	// ����� ���������� � ������ ��������� ������ �����, ����:
	// - ����������� ������� �������, ����� ����������� ������� �����������
	// - ������������ ��������� ���������� ������� typename
	
	// �������� ������� ������� � ������� �����������
	int b = ::max(1, 3.5);
	// warning C4244: 'argument' : conversion from 'double' to 'const int', possible loss of data

	// �������� ������ (����� <> - ����� ����� �������, � �� �������)
	b = ::max<>(1, 2);
}

// ������������ �������� ������� ������
void show_class_tmpl(){
	stack<int> s;

	s.push(1);
	s.push(2);

	// ��������������� ���������� ������ ��� ���������� �������-������
	int i = s.top();
	s.pop();
	// �.�. bool empty() �� ��������������

	stack<string> s2;
	s2.push("atatat1");
	s2.push("atatat2");
}

// ������������ ��������� �������������
void show_parital_spec(){
	double_stack<long, int> ds0;	// ������������� "������ �������� - int"
	double_stack<int, double> ds1;	// ��� �������������
	double_stack<double, double> ds2;	// ������������� "��������� ����������"
	double_stack<double*, int*> ds3;	// ������������� "��������� - ���������"
}

// ������������ ���������� �� ���������
void show_default_param(){
	custom_stack<int> st1;
	custom_stack<int, std::deque<int> > st2;

	st1.push(1);
	st2.push(1);
}

// ������������ ����������, �� ���������� ������ 
// (������ ������-������� �������������� �������)
void show_non_type_params(){

	// ��� ������ ����!
	fixed_array<int, 20> ar1;
	fixed_array<int, 40> ar2;

	try {
		ar1[10] = 10;
		ar2[20] = 10;
		// ����� ��������� �������� ������, ������ ����������
		ar2[200] = 10;
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;		
	}
}

// ������������ ��������� �������� ���� typename � template
void show_typename_tmpl(){
	std::list<int> v;
	v.push_back(1);
	v.push_back(2);

	print_container< std::list<int> >(v);

	test_bitset();
}

// ������������� ��������� ����� ���������� ������
void show_template_member(){
	copyable_stack<int> is;
	copyable_stack<double> ds;

	is.push(1);
	ds.push(1.0);
	ds.push(2.0);
	is = ds;
}

// ������������ �������� � ��������� ����������
void show_tmpl_param(){
	
	// ��������� � ���������� �� ���������
	tmpl_stack<int> s1;
	s1.push(1);
	s1.push(2);

	// ���������� ������ ��� � ���������
	tmpl_stack<double, std::vector> s2;
	s2.push(1.0);
	s2.push(3.0);

	// ������������ operator= (����� ������� ���� ������)
	s1 = s2;

	// ��� �������� ������� ��������� ��������� �� �����������!
}


// ������������ ������������� �����
void show_zero_init(){
	
	// ��� ���� ����� ����� ����������� ������������� �����
	init_zero<int> iz1;
	init_zero< std::vector<int> > iz2;
	init_zero< std::string > iz3;
}

// ������������ ��������� ��������� � ���������� �������� � ����������
// �������������� ������� � ��������� ��������� ������ � ������, ���� ��� ������ ���������� �� ��������
void show_literal(){

	// ������: ������ �������� ������ � ���������, �.�. ���������� �� ������
	// error C2440: 'return' : cannot convert from 'const char *' to 'const char (&)[5]'
	// str_max_ref("aaaa", "bbbb");
	show_ref("aaaa");

	// � ����� ����� �������� ������ � ���������, �.�. ���������� �� ��������
	str_max_val("aaaa", "bbbb");
	show_val("aaaa");

	//��� �������������� �������� ��������:
	// - ������������ �������� �� ������ (��� ���������� ��� ��������)
	// - �� ������������ �-������ (���������������)
	// ������ ������ �������� � std::make_pair(f, s) - ��� ��������� �� �������� ��� �������� ���������
}

int main(){
	
	show_func_template();
	show_class_tmpl();
	show_parital_spec();
	show_default_param();
	show_non_type_params();
	show_typename_tmpl();
	show_tmpl_param();
	show_literal();
	
	
	return 0;
}