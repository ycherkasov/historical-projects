#pragma once
#include "simple_array.h"
#include "expressions.h"

template <typename T, typename Rep = SArray<T> >
class Array{
private:
	// ������������ ������ (������������� �������)
	Rep expr_rep;
public:

	// ������ ���������� �������
	explicit Array(size_t s): expr_rep(s){}

	// ������ �� ������������� �������������
	explicit Array(Rep const& r): expr_rep(r){}

	// �������� ���������� ��� ������ ����
	Array& operator=(Array const& rhs){

		print_debug("=");
		assert(size() == rhs.size());
		for(size_t i = 0; i < size(); ++i){
			expr_rep[i] = rhs[i];
		}
		return *this;
	}

	// �������� ���������� ��� ������� ����
	template <typename T2, typename Rep2>
	Array& operator=(Array<T2, Rep2> const& rhs){

		print_debug("=");
		assert(size() == rhs.size());
		for(size_t i = 0; i < size(); ++i){
			expr_rep[i] = rhs[i];
		}
		return *this;
	}

	// ������ �������������
	size_t size() const {return expr_rep.size();}

	// ��������� ��������������
	T operator[](size_t i) const {return expr_rep[i];}

	T& operator[](size_t i) {return expr_rep[i];}

	Rep& rep() {return expr_rep;}
	Rep const& rep() const {return expr_rep;}

};

// ��������� ������������� ������� ��������� ����������
// ������ ��������� �������� ��������� ����� ���������� ����� ��������


// �������� �������� (������� �� ��������)
template <typename T, typename R1, typename R2>
Array<T, A_Add<T, R1, R2> >		// return value
operator+(Array<T, R1> const& a, Array<T, R2> const& b){

	return Array<T, A_Add<T, R1, R2> >( A_Add<T, R1, R2>(a.rep(), b.rep()) );
}

// ��������� �������� (������� �� ��������)
template <typename T, typename R1, typename R2>
Array<T, A_Mult<T, R1, R2> >		// return value
operator*(Array<T, R1> const& a, Array<T, R2> const& b){

		return Array<T, A_Mult<T, R1, R2> >( A_Mult<T, R1, R2>(a.rep(), b.rep()) );
}

// ��������� ������� �� ������ (������� �� ��������)
template <typename T, typename R2>
Array<T, A_Mult<T, A_Scalar<T>, R2> >		// return value
operator*(T const& a, Array<T, R2> const& b){

		return Array<T, A_Mult<T, A_Scalar<T>, R2> >( A_Mult<T, A_Scalar<T>, R2>( A_Scalar<T>(a), b.rep()) );
}
