#include "rsdn_example.h"
#include "exceptions.h"
#include "namespaces.h"
#include "tests.h"

int main()
{
	// ���������� 8.10 �� ����������
	test_some_overflow();
	test_ex();
	show_namespaces();
	show_exceptions();
	return 0;
}