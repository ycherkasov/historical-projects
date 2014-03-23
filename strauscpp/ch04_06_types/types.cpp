#include <iostream>
#include "bit_operations.h"
/*
–азмеры типов:
1 = sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long)
1 <= sizeof(bool) <= sizeof(long)
sizeof(char) <= sizeof(wchar_t) <= sizeof(long)
sizeof(float) <= sizeof(double) <= sizeof(long double)
sizeof(N) <= sizeof(signed N) <= sizeof(unsigned N)
*/

using std::cout;
using std::endl;

/* ѕеречислени€ и их размеры */
enum enum1{dark, light};		// диапазон 0:1
enum enum2{ a = 3, b = 9 };		// диапазон 0:15
enum enum3{ c = 3, d = 900 };	// диапазон 0:1024
enum enum4{ e = 3, f = 5000000000 };	// диапазон 0:2^32 - не входит!

void show_enumerations(){

	// преобразование в enum1
	enum1 e1 = enum1(0);
	enum1 e2 = enum1(1);

	enum1 e3 = enum1(20); // - компилируетс€, но неверно -
	// 20 не принадлежит перечислению enum1
	
	// –азмер перечислени€ архитектурозависим
	size_t sz = sizeof(enum1);
	sz = sizeof(enum2);
	sz = sizeof(enum3);

	// «десть значение enum будет обрезано!
	enum4 e4 = enum4::f;
	sz = sizeof(e4);
	long long l = e4;
}

static int g_global = 5;

void show_name_convensions(int a){
	// ƒавайте переменным с большой областью видимости 
	// большие, пон€тные имена
	static int test_counter = a;

	// ј с малой - короткие однобуквенные
	for ( int i = 0 ; i < test_counter ; i++ ){
		::g_global += test_counter;
		cout << ::g_global << endl;
	}

	// обращение к глобальной
	::g_global = 0;

	// сокрытие глобальной 
	int g_global = 0; // (автоматический объект)

	// Ќе существует способа обратитьс€ к 
	// сокрытой локальной переменной!
}

void show_pointer(){

	// как работает typedef дл€ массива
	typedef int MyNumbers[10];
	MyNumbers numbers = {1,2,3};

	// √арантируетс€, что нет объектов с нулевым адресом.

	// замещение макроса NULL (нулевой указатель)
	//const void* NULL = 0;
	const int* m;
	const int*& o = m;

	// строковый литерал - это указатель
	size_t sz = sizeof("aaa");

	// строковый литерал по умолчанию константен
	char* p = "literal";

	// дл€ формировани€ неконстантной строки используйте массив
	char p_arr[] = "array";
	p_arr[0] = 'A';


	// јрифметика указателей:
	// ¬ычитание числа - перемещение по массиву назад
	// ѕрибавление числа - перемещение по массиву вперед
	// ¬ычитание указателей - рассто€ние между ними в элементах
	// —ложение указателей - не имеет смысла и запрещено

	// –езультатом sizeof €вл€етс€ беззнаковый int = size_t (cstddef)
	// “ипом разности указателей - int = ptrdiff_t (cstddef)

	// Interesting notation of string arrays
	int x = 2;
	cout << x["ABCDEF"] << endl;
	cout << 2["ABCDEF"] << endl;
	
	// error C2166: l-value specifies const object
	//x["ABCDEF"] = 'Z';

	// correct!
	x[p_arr] = 'Z';
}

void show_references(){
	
	int ii = 10;

	// ссылку нужно инициализировать
	// перенаправить ее уже нельз€
	int& rr = ii;

	// ссылки на константы более предпочтительны
	// т.к. неконстантный объект более подвержен ошибкам
	const int ic = 0;
	const int& rc = ic;

	// ссылки можно возвращать - тогда возвращающа€ функци€ будет lvalue
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
	show_bits();
	show_enumerations();
	show_name_convensions(5);
	show_pointer();
	show_references();
	show_ifs(-1);

	return 0;
}
