#pragma once
#include "basic_types.h"
#include "compound_types.h"
#include "compound_types.h"
//#include "enum_types.h"

// ����������, �������� �� ��� �������, ����� ��� ������ SFINAE (��. ������ ����� - ������� �� �������� :) )
// ��� �� ���� �� �� �������� �� ����� �� ������� � ��������� �����

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