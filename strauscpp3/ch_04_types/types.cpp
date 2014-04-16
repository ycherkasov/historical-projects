#include <iostream>
#include "bit_operations.h"
/*
������� �����:
1 = sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long)
1 <= sizeof(bool) <= sizeof(long)
sizeof(char) <= sizeof(wchar_t) <= sizeof(long)
sizeof(float) <= sizeof(double) <= sizeof(long double)
sizeof(N) <= sizeof(signed N) <= sizeof(unsigned N)
*/

using std::cout;
using std::endl;

/* ������������ � �� ������� */
enum enum1{dark, light};		// �������� 0:1
enum enum2{ a = 3, b = 9 };		// �������� 0:15
enum enum3{ c = 3, d = 900 };	// �������� 0:1024
enum enum4{ e = 3, f = 5000000000 };	// �������� 0:2^32 - �� ������!

void show_enumerations(){

	// �������������� � enum1
	enum1 e1 = enum1(0);
	enum1 e2 = enum1(1);

	enum1 e3 = enum1(20); // - �������������, �� ������� -
	// 20 �� ����������� ������������ enum1
	
	// ������ ������������ ������������������
	size_t sz = sizeof(enum1);
	sz = sizeof(enum2);
	sz = sizeof(enum3);

	// ������ �������� enum ����� ��������!
	enum4 e4 = enum4::f;
	sz = sizeof(e4);
	long long l = e4;
}

static int g_global = 5;

void show_name_convensions(int a){
	// ������� ���������� � ������� �������� ��������� 
	// �������, �������� �����
	static int test_counter = a;

	// � � ����� - �������� �������������
	for ( int i = 0 ; i < test_counter ; i++ ){
		::g_global += test_counter;
		cout << ::g_global << endl;
	}

	// ��������� � ����������
	::g_global = 0;

	// �������� ���������� 
	int g_global = 0; // (�������������� ������)

	// �� ���������� ������� ���������� � 
	// �������� ��������� ����������!
}

void show_pointer(){

	// ��� �������� typedef ��� �������
	typedef int MyNumbers[10];
	MyNumbers numbers = {1,2,3};

	// �������������, ��� ��� �������� � ������� �������.

	// ��������� ������� NULL (������� ���������)
	//const void* NULL = 0;
	const int* m;
	const int*& o = m;

	// ��������� ������� - ��� ���������
	size_t sz = sizeof("aaa");

	// ��������� ������� �� ��������� ����������
	char* p = "literal";

	// ��� ������������ ������������� ������ ����������� ������
	char p_arr[] = "array";
	p_arr[0] = 'A';


	// ���������� ����������:
	// ��������� ����� - ����������� �� ������� �����
	// ����������� ����� - ����������� �� ������� ������
	// ��������� ���������� - ���������� ����� ���� � ���������
	// �������� ���������� - �� ����� ������ � ���������

	// ����������� sizeof �������� ����������� int = size_t (cstddef)
	// ����� �������� ���������� - int = ptrdiff_t (cstddef)

	// Interesting notation of string arrays
	int x = 2;
	cout << x["ABCDEF"] << endl;
	cout << 2["ABCDEF"] << endl;
	
	// error C2166: l-value specifies const object
	//x["ABCDEF"] = 'Z';

	// 1[array] == array[1], ����� ������ 
	// 1 + *pointer == *pointer + 1
	// correct!
	x[p_arr] = 'Z';

	// ��� �� ����� C, ��� �������� 1[x] ������������ ����� *(1 + x)
	// ISO / IEC 9899:TC2 : 6.5.2.1 Array subscripting

	// Thus, &*E is equivalent to E (even if E is a null pointer), and &(E1[E2]) to ((E1)+(E2))
	
	// ...a pointer to a nonarray object behaves the same as a pointer to the
	// first element of an array of length one with the type of the object as its element type
}


//http://rsdn.ru/forum/cpp/5458937.flat#5458937
// ������ ���������������� �����������. 
// ������-�� ����� �������, ��� ���� ������ ������ ������������� � ���������, 
// �� ����� ������� � ��� ������ � ��� ���� � ����
// ���. ��������� &array � array ����� ������ ����, �� ���������� ���� ���������.


// ��� ����������� ������� � show_array_pointers()
int stack_address_increment(int i)
{
	// �������� �� ����� �� �������� �������� �� i (0)
	int a = 0[&i];

	// �������� �� ����� �� �������� 1 �� i (0xcccccccc � msvc)
	int b = 1[&i];

	// Memory dump 0[&i]       1[&i] ...
	// 0x002CF9E8  00 00 00 00 cc cc cc cc cc cc cc cc 00 00 00 00 00 fa 2c 00 c8 56 26 01 50
	// 0x002CFA01  fa 2c 00 89 64 26 01 01 00 00 00 28 81 49 00 70 6e 49 00 8c 37 2a 52 00 00

	// ���������� ������ �� �������� ���������,
	// ���������� � �������

	// ��������� *(1 + &i) � 1[&i] ������������
	return 1[&i];
	//return *(&i + 1);
}

// ������ �������
// ��� ����������� ������� � show_array_pointers()
template <typename T, std::size_t N>
inline std::size_t countof(T(&arr)[N]) {
	return N;
}


void show_array_pointers(){
	
	// 1.
	int i = 0;
	// ������ ���������� �� �������� + 1 �� �����
	int ii1 = stack_address_increment(i);
	int ii2 = stack_address_increment(1);

	// 2.
	// ������ � ��������� �� ������ - ������ ����, �� ������ ���� ����������
	// ��� ����� ���������� ���� � �����, �� ���������� �� ������

	int array[42] = {};

	int* ii3 = array;

	// ������ ������� ���� � ��� ���!
	// cannot convert from 'int (*)[42]' to 'int *'
	// int* ii31 = &array;
	int(*ii4)[42] = &array;

	// ������, ��� ��� ���� � ��� �� ���������
	ptrdiff_t diff1 = reinterpret_cast<int*>(ii4)-ii3;

	std::cout << std::hex << ii4 << '-' << ii3 << '=' << std::dec << diff1 << '\n';

	// 3.
	// �� ���� ����������� ��������� 1, ������� ���������� ������
	int* ii5 = 1 + array;

	// cannot convert from 'int (*)[42]' to 'int *'
	// int* ii31 = 1 + &array;
	int(*ii6)[42] = 1 + &array;

	// diff2 ����� 41. �.�. (&array + 1) ��������� �� ��������� ������� + �������� 1
	ptrdiff_t diff2 = reinterpret_cast<int*>(ii6) - ii5;

	std::cout << std::hex << ii6 << '-' << ii5 << '=' << std::dec <<  diff2 << '\n';

	// 4.
	// ������������ ��� ���������� �������, ����� �������� ������ �������
	// &array+1 ����� ��������� �� �������, ��������� �� ��������
	// &array[0] - �� ������ �������
	ptrdiff_t diff3 = (1[&array] - &array[0]);

	// ���������� ��������� ���������� ��������� �������:
	// 1( [&array] ) - &( array[0] ) == ( 1 + &array ) - &(0 + array)

	std::cout << std::hex << 1[&array] << '-' << &array[0] << '=' << std::dec << diff3 << '\n';

	// 5.
	// �� ����� ������������ compile-time ����������, ��� ����� ������ ����� � ���
	std::cout << countof(array) << '\n';
}


void show_references(){
	
	int ii = 10;

	// ������ ����� ����������������
	// ������������� �� ��� ������
	int& rr = ii;

	// ������ �� ��������� ����� ���������������
	// �.�. ������������� ������ ����� ��������� �������
	const int ic = 0;
	const int& rc = ic;

	// ������ ����� ���������� - ����� ������������ ������� ����� lvalue
}

// This is correct.  The else actually matches with the second if (C++ Standard 6.4.1/1).
// show_ifs(-1) ret 0
int show_ifs(int x){
	if(x > 0)
		if(x > 100)
			return 1;
	else
		return 2;
	return 0;
}





int main(){

	// example from rsdn
	show_array_pointers();

	show_bits();
	show_enumerations();
	show_name_convensions(5);
	show_pointer();
	show_references();
	show_ifs(-1);

	return 0;
}
