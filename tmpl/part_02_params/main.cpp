#include "tmpl_members.h"
#include "non_type_params.h"
#include "template_params.h"
#include "arguments.h"
#include "sfinae.h"
#include "friends.h"

// �������� ������� �����������
// ��������, �����:
template <typename T>
union alloc_chunc{
	T object;
	unsigned char bytes[sizeof(T)];
};

void test_union_template(){
	alloc_chunc<int> ich = {};
	alloc_chunc<double> dch = {};
}


void test_members(){
	tmpl_members<int> i(5);
	tmpl_members<double>::self_tmpl<double> d(5.0);
}



void test_virtual(){
	base<int> b;
	b.open(5);

	derived<double> d;
	base<double>* pd = &d;
	d.open(7);
}

void test_tmpl_params(){
	tmpl_stack<int> s;
}

void test_arguments(){
	// � ���� ������ ��������� ������ �������� �������,
	// �� ���� ����������� ��������
	int i = implicit_cast<int>(2.0);

	// � � ���� - �� ���� �� ���������,
	// �.�. ��� ������������� �������� ������ � ������
	double d = implicit_cast2<int, double>(2);
}

void test_sfinae(){
	/*RT1*/RT2 rt1 = test_sfinae_x<test_X>();
	size_t sz1 = sizeof(rt1);

	RT2 rt2 = test_sfinae_x<char>();
	size_t sz2 = sizeof(rt1);

	bool b = type_has_member_type_X(test_X);
	b = type_has_member_type_X(char);
	
 	test_sfinae_int();
}

int main(){
	test_union_template();
	test_members();
	test_virtual();
	test_functions_family();
	test_arguments();
	test_sfinae();
	test_argument_types();
	test_friends_odr();

	return 0;
}
