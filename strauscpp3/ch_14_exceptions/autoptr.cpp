#include <memory>
#include "autoptr.h"
using std::auto_ptr;

// ������ ������������ ������ ���������
// � ������� ����� ����������, �� ��������� ���������, ������������ �� ���������� ������ ��������� explicit
// � std::auto_ptr ����������� ��������� ������ ������, � �� ����������� (��� �������������� �����������)
// ���������� ����������� � ����������� ������� ��� ���� ������ �� ������������

// ������, � ���������� ����� ��������� ���������� ����������� ����� ������ ������� ��������. 
// ���, ��� ������� ������� �������� � �������, ������������� lvalue, 
// ����� ������������������ ��� ��������� ���� ����������� ������ ������� move

// �� ����������� ������� �������������� ������ ��������� � �������!

void test_autoptr(){

	struct test_auto
	{
		test_auto() : a(), b(){}
		int a;
		double b;
	};

	auto_ptr<test_auto> ap1 (new test_auto() ) ;

	test_auto* pa = new test_auto();
	
	// �������� �� ��������
	auto_ptr<test_auto> ap2(pa);

	// �������� �������� ap3 (ap2 ���������� ���)
	auto_ptr<test_auto> ap3(ap2);

	// ���� ������� ��������� ���������� �������� 
	// (��� �������������� �����������)

	// ������: ap2 ��� ������� �������� pa
	//ap2->a = 6;

	// �������� ��������� �� ��������� ������
	const test_auto* pa1 = ap3.get();

	// ������ - �������� �� �������� �������� ������ (UB)
	// auto_ptr<test_auto> ap4(pa);

	// ����������� auto_ptr - �� ������ �� �.�. �������
	// �� � �� ����� �������� ����� �� �������� ����������
	const auto_ptr<test_auto> apconst(new test_auto());
	
	// ������: �.�. ����������� ���������� const auto_ptr 
	// �������� �� ����, ��� ���������
	// ap2 = apconst;

	// ��� ����������� � ������� ���������������� ���� auto_ptr_ref

	// ������ ������������ � �������� ��������� ���������� ����������� ��������� auto_ptr,
	// ������ ��� �������� ����������� auto_ptr �������� �������� ���������
}