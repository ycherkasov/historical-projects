#pragma once

// ��������� ���� - ��� �������, �������� � ������

// ������� ������ ��� ���� ����� - ���������
template <typename T>
struct is_pointer{
	static bool result(){return false;}
};

// ������� ������ ��� ���� ����� - ������
template <typename T>
struct is_reference{
	static bool result(){return false;}
};

// ������� ������ ��� ���� ����� - ������
template <typename T>
struct is_array{
	static bool result(){return false;}
};

// ������� ������ ��� ���� ����� - ��������� �� �������
template <typename T>
struct is_function_pointer{
	static bool result(){return false;}
};

// ������� ������ ��� ���� ����� - ��������� �� �����
template <typename T>
struct is_method_pointer{
	static bool result(){return false;}
};

//////////////////////////////////////////////////////////////////////////

// ������������� - ���������
template <typename T>
struct is_pointer<T*>{
	static bool result(){return true;}
};

// ������������� - ������
template <typename T>
struct is_reference<T&>{
	static bool result(){return true;}
};

// ������������� - ������
template <typename T, int N>
struct is_array<T[N]>{
	static bool result(){return true;}
};

// ����� ��� ������� ��� �������� ���������� ���������
template <typename T>
struct is_array<T[]>{
	static bool result(){return true;}
};

// ������� ������ ��� ���� ����� - ��������� �� �������
// ��� ����������
template <typename T>
struct is_function_pointer<T()>{
	static bool result(){return true;}
};

// � 1 ����������
template <typename T, typename P>
struct is_function_pointer<T(P)>{
	static bool result(){return true;}
};

// � 2 � ����� ����������
template <typename T, typename P1>
struct is_function_pointer<T(P1, ...)>{
	static bool result(){return true;}
};

// ������� ������ ��� ���� ����� - ��������� �� �����
template <typename T, typename C>
struct is_method_pointer<T C::*>{
	static bool result(){return true;}
};


//////////////////////////////////////////////////////////////////////////
// TODO: is_function, other methods: SFINAE, * to &
