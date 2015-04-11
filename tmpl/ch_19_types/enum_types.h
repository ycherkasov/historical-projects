#pragma once
#include "basic_types.h"
#include "compound_types.h"

// Для определения перечисления можно воспользоваться их свойством 
// неявного преобразования в интегральный тип

// Для проверки раземра возвращаемого значения
struct size2{char c[2];};

template <
	typename T, 
	bool convert_possible = !is_function_pointer<T>::result() && !is_array<T>::result() 
>
struct consumeUDC{
	operator T() const {}
};

template <typename T>
struct consumeUDC<T, false>{};

template <bool convert_possible>
struct consumeUDC<void, convert_possible>{};

char enum_check(bool);
char enum_check(char);
char enum_check(wchar_t);
char enum_check(unsigned char);

char enum_check(short);
char enum_check(unsigned short);

char enum_check(long);
char enum_check(unsigned long);

char enum_check(int);
char enum_check(unsigned int);

char enum_check(float);
char enum_check(double);
char enum_check(long double);

size2 enum_check(...);

template <typename T>
struct is_enum{
	static bool result(){
		return !is_fundamental<T>::result()
			&& !is_reference<T>::result()
			&& !is_pointer<T>::result()
			&& !is_method_pointer<T>::result()
			&& (1 == sizeof( enum_check( consumeUDC<T>() ) ) );
	}
};