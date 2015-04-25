#include <iostream>
#include "basic_types.h"
#include "compound_types.h"
#include "enum_types.h"
#include "class_types.h"

using std::cout;
using std::endl;

struct A{
	A():i(){}
	int i;
	void f(){(i++);}
};

enum atatat{
	atata1 = 1,
	atata2 = 1,
};

void show_fundamental_types(){
	bool b = is_fundamental<int>::result();
	cout << "int is fundamental = " << b << endl;

	b = is_fundamental<char>::result();
	cout << "char is fundamental = " << b << endl;

	b = is_fundamental<A>::result();
	cout << "A is fundamental = " << b << endl;
}

void show_compound_types(){

	typedef void(*void_func_t)();
	typedef void(A::* p_member_t)();

	bool b = is_pointer<int*>::result();
	cout << "int* is pointer = " << b << endl;

	b = is_pointer<int>::result();
	cout << "int is pointer = " << b << endl;

	b = is_reference<int&>::result();
	cout << "int& is_reference = " << b << endl;

	b = is_reference<int>::result();
	cout << "int is_reference = " << b << endl;

	b = is_array<int[5]>::result();
	cout << "int[5] is_array = " << b << endl;

	b = is_array<int[]>::result();
	cout << "int[] is_array = " << b << endl;

	b = is_array<int>::result();
	cout << "int is_array = " << b << endl;

	// для функций не работает(???)
	b = is_function_pointer<void_func_t>::result();
	cout << "void_func_t is_function_pointer = " << b << endl;

	b = is_function_pointer<int>::result();
	cout << "int is_function_pointer = " << b << endl;

	b = is_method_pointer<p_member_t>::result();
	cout << "A::f is_method_pointer = " << b << endl;

	b = is_method_pointer<int>::result();
	cout << "int is_method_pointer = " << b << endl;
}

#if 0
void show_is_enum(){
	bool b = is_enum<atatat>::result();
	cout << "atatat is_enum = " << b << endl;

	b = is_enum<int>::result();
	cout << "int is_enum = " << b << endl;

	b = is_enum<A>::result();
	cout << "A is_enum = " << b << endl;
}
#endif

void show_class_types(){
	bool b = is_class<A>::result();
	cout << "A is_class = " << b << endl;

	b = is_class<int>::result();
	cout << "int is_class = " << b << endl;
}


int main(){

	show_fundamental_types();
	show_compound_types();
	//show_is_enum();
	show_class_types();

	return 0;
}