#pragma once
#include "basic_types.h"
#include "compound_types.h"
#include "compound_types.h"
//#include "enum_types.h"

// Определить, является ли тип классом, можно при помощи SFINAE (см. пример ранее - который не работает :) )
// Или же если он не является ни одним из базовых и составных типов

template <typename T>
struct is_class{
	static bool result(){
		return 
			!is_fundamental<T>::result() &&
			//!is_enum<T>::result()
			!is_pointer<T>::result() &&
			!is_reference<T>::result() &&
			!is_array<T>::result() &&
			//!is_function_pointer<T>::result() &&
			!is_method_pointer<T>::result();
	}
};