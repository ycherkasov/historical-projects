#pragma once
#include "if_then_else.h"

// �������� ������ ��� ������ �����
// ������ ������ ����� ������ �������� ������� � ������ ����������
// �����, �� ������� ������ �����, ���������� -1
// ��� ������� N=16 ������� ���������� ���������
// Sqrt<16, 1, 16>
// Sqrt<16, 1, 8>
// Sqrt<16, 1, 4>
// Sqrt<16, 3, 4>
// Sqrt<16, 4, 4> => sqrt(16) = 4

template <int N, int L0 = 1, int HI = N>
struct Sqrt{
	enum { mid = (L0+HI+1)/2 };

	typedef typename 
		IfThenElse< 
			(N < (mid*mid)), 
			Sqrt<N, L0, mid-1>, 
			Sqrt<N, mid, HI> 
		>::Result CalculatedT;

	enum { result = CalculatedT::result };
};

template <int I>
struct TypeWrapper
{
    enum { result = I };
};


// ��������� ������������� ��� ���������� ��������
// ������� � ������ ��������� ��������� - ������� ���������
template <int N, int M>
struct Sqrt<N, M, M>{

    typedef typename
    IfThenElse <
    (N == (M*M)),
    typename TypeWrapper<M>,
    typename TypeWrapper<-1>
    >::Result CalculatedT;


	enum {result = CalculatedT::result };
};
