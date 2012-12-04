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

	//enum1 e3 = enum1(20); - �������������, �� ������� -
	// 20 �� ����������� ������������ enum1
	
	// ������ ������������ ������������������
	size_t sz = sizeof(enum1);
	sz = sizeof(enum2);
	sz = sizeof(enum3);

	// ������ �������� enum ����� ��������!
	//enum4 e4 = enum4::f;
	//sz = sizeof(e4);
	//long long l = e4;
}

static int g_global = 5;

void show_name_convensions(int a){
	// ������� ���������� � ������� �������� ��������� 
	// �������, �������� �����
	static int test_counter = a;

	// � � ����� - �������� �������������
	for ( int i = 0 ; i < test_counter ; i++ ){
		::g_global += test_counter;
	}

	// ��������� � ����������
	::g_global = 0;

	// �������� ���������� 
	int g_global = 0; // (�������������� ������)

	// �� ���������� ������� ���������� � 
	// �������� ��������� ����������!
}

void show_pointer(){

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

// test results

void todo(){
	int x = 2;
	cout << x["ABCDEF"] << endl;

	// ������� � ���������

	// The variable declared inside the switch without braces is valid as well 
	// because there is no case label which would jump over it.

	// The language does not allow for changing operator precedence or associativity.

	// A default constructor may have parameters that have default values.



	// illegal
	//int aa[5,4];

}

// This is correct.  The else actually matches with the second if (C++ Standard 6.4.1/1).
// f(-1) ret 0
int f1(int x){
	if(x > 0)
		if(x > 100)
			return 1;
	else
		return 2;
	return 0;
}

namespace{
	int i;
}
// "i" has external linkage.
// This is correct.  Even though "i" is not visible outside the compilation unit, 
// it still has external linkage (C++ Standard 7.3.1.1/1 - See footnote).

// short is system dependent

// Deep copying classes
// A copy constructor, assignment operator, and a virtual destructor
// This is the correct answer.  It is essential to remember these requirements.  (Stroustrup 10.4.1, 10.4.4.1, 12.4.2.)

/*
Which type of construct do you use to logically group names that can be shared across multiple source files?
ANSI/ISO C++ Standard (7.3).

This is the purpose of namespaces.  
They provide logical groupings of identifiers.  
The result of using a namespace is similar to what could be accomplished using a class 
with all static members.  
However, unlike a class, a namespace can be reopened to include additional identifiers.
*/

//std::unexpected
//ANSI/ISO C++ Standard (15.4/8, 15.4/10, 15.5.2/1).

// !(!a && !b)
//a || b
//This is correct.  NOT (NOT A  AND  NOT B) equals A OR B.  Conversely, NOT (NOT A  OR  NOT B) equals A AND B.

//X* x = new Y[2];
// delete[] x; 
//It invokes undefined behavior.
//This is correct.  
//When deleting an array, the dynamic and the static type of the object must be the same, 
//or the behavior is undefined (C++ Standard 5.3.5/3).

/*
print("%08x\n", 42);
cout << setw(8) << setfill('0') << hex << 42 << endl;
This is the correct answer.  
The field width is set to 8, padding is set to the character 0 
and the output for integers is set to hexadecimal.  
This is the exact equivalent of the "%08x" format specification in the "printf()" C library function.
*/

/*
cin >> line1;
getline(cin, line2);
cout << "EOF\n";

It will print out: 
Line 1: Hello 
Line 2:  world! 
EOF
This is the correct answer.  
Although std::skipws is issued in the stream, 
std::getline() bypasses that mechanism, 
and the space that terminated the extraction will be read in.  
In addition, std::getline() never keeps the terminating newline in the destination string.
*/

int main(){
	show_bits();
	show_enumerations();
	show_name_convensions(5);
	show_pointer();
	show_references();
	return 0;
}
