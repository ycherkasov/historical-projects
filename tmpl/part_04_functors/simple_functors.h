#pragma once
#include <vector>
#include <algorithm>

// ������� ���������� �� �������� ��� �������
template <typename T>
struct my_less{
	bool operator()(T const& a, T const& b){
		return a > b;
	}
};


// ������� ����� ���� ��� ���������� �������, ��� � ���������� �������
template <typename Func>
void custom_sort(std::vector<int>& sort_me, Func predicate = Func()){
	std::sort(sort_me.begin(), sort_me.end(), predicate);
}

//////////////////////////////////////////////////////////////////////////
// ���� �� ����� � �������� ������� ��������� �� �������,
// ����� �������� �� � ������� (������� ���������� ��������� � �.�.)

// ������� ��������� ��������������� ������ 
// ��� ������� ������� � ������ ������ ����������

// ��������� ������ - ��� ������� � 3 �����������
template <typename RT, typename T1 = void, typename T2 = void, typename T3 = void>
struct function_ptr_t{
	enum { num_params = 3};
	typedef RT (*FType)(T1,T2,T3);
};

// ��������� ������������� ��� ������� � 2 �����������
template <typename RT, typename T1, typename T2>
struct function_ptr_t<RT, T1,T2,void>{
	enum { num_params = 2};
	typedef RT (*FType)(T1,T2);
};

// ��������� ������������� ��� ������� � 1 ����������
template <typename RT, typename T1>
struct function_ptr_t<RT, T1,void,void>{
	enum { num_params = 1};
	typedef RT (*FType)(T1);
};

// ��������� ������������� ��� ������� ��� ����������
template <typename RT>
struct function_ptr_t<RT, void,void,void>{
	enum { num_params = 0};
	typedef RT (*FType)();
};

//////////////////////////////////////////////////////////////////////////
// ������ ������������ ��������� IfThenElse,
// ������������ ��� � ����������� �� �������

// ��������� ������ - �� ����� �����������
template <bool B, typename Tr, typename Fls>
struct IfThenElse;

// ��������� ������������� ��� True
template <typename Tr, typename Fls>
struct IfThenElse<true, Tr, Fls>{
	typedef Tr Result;
};

// ��������� ������������� ��� False
template <typename Tr, typename Fls>
struct IfThenElse<false, Tr, Fls>{
	typedef Fls Result;
};


//////////////////////////////////////////////////////////////////////////
// ��������������� �����, ����� "������" ��� void � ����������
// �������� ��� ��������� �� ����������� ������
template <typename T>
struct forward_param{
	typedef typename T const& Type;
};

// ����� void, ��� ���� - �������������
template <>
struct forward_param<void>{
	struct Unused{};
	typedef Unused Type;
};

// ��� ����� ��������
template <typename RT, typename T1 = void, typename T2 = void, typename T3 = void>
class function_ptr{
	
	typedef typename function_ptr_t<RT,T1,T2,T3>::FType func_ptr;
	enum { num_params = function_ptr_t<RT,T1,T2,T3>::num_params };

public:

	// ������������� ��������� �� �������
	function_ptr(func_ptr ptr):_fptr(ptr){}

	// ������������� ������
	RT operator()(){
		return _fptr();
	}

	RT operator()(typename forward_param<T1>::Type a1){
		return _fptr(a1);
	}

	RT operator()(typename forward_param<T1>::Type a1,
		typename forward_param<T2>::Type a2){
		return _fptr(a1, a2);
	}

	RT operator()(typename forward_param<T1>::Type a1, 
		typename forward_param<T2>::Type a2, 
		typename forward_param<T3>::Type a3){

		return _fptr(a1, a2, a3);
	}

private:
	func_ptr _fptr;	
};
