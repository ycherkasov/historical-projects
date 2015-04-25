#pragma once
#include "simple_array.h"
#include "expressions.h"

// ѕараметр Rep в данном шаблоне может быть как массивом, так и 
// шаблоном "отложенных вычислений", 
// который валидируетс€ только при обращении к operator[]
template <typename T, typename Rep = SArray<T> >
class Array{
private:
	// »нкапсул€ци€ данных (представление массива)
	Rep expr_rep;
public:

	// ћассив указанного размера
	explicit Array(size_t s): expr_rep(s){}

	// ћассив из существующего представлени€
	explicit Array(Rep const& r): expr_rep(r){}

	// ќператор присвоени€ дл€ тогоже типа
	Array& operator=(Array const& rhs){

		print_debug("=");
		assert(size() == rhs.size());
		for(size_t i = 0; i < size(); ++i){
			expr_rep[i] = rhs[i];
		}
		return *this;
	}

	// ќператор присвоени€ дл€ другого типа
	template <typename T2, typename Rep2>
	Array& operator=(Array<T2, Rep2> const& rhs){

		print_debug("=");
		assert(size() == rhs.size());
		for(size_t i = 0; i < size(); ++i){
			// при каждом обращении к [] будет один раз вычислено выражение
			expr_rep[i] = rhs[i];
		}
		return *this;
	}

	// размер представлени€
	size_t size() const {return expr_rep.size();}

	// операторы индексировани€
	T operator[](size_t i) const {return expr_rep[i];}

	T& operator[](size_t i) {return expr_rep[i];}

	Rep& rep() {return expr_rep;}

	Rep const& rep() const {return expr_rep;}
};

// –еализуем перегруженные функции векторной арифметики
// теперь благодар€ шаблонам выражений часть вычислений будет свернута


// —ложение векторов (возврат по значению)
template <typename T, typename R1, typename R2>
Array<T, A_Add<T, R1, R2> >		// return value
operator+(Array<T, R1> const& a, Array<T, R2> const& b){
	// здесь не вычисл€етс€ выражение, а только формируетс€ шаблон,
	// вычисл€ющий его по запросу в operator[]
	return Array<T, A_Add<T, R1, R2> >( A_Add<T, R1, R2>(a.rep(), b.rep()) );
}

// ”множение векторов (возврат по значению)
template <typename T, typename R1, typename R2>
Array<T, A_Mult<T, R1, R2> >		// return value
operator*(Array<T, R1> const& a, Array<T, R2> const& b){

		return Array<T, A_Mult<T, R1, R2> >( A_Mult<T, R1, R2>(a.rep(), b.rep()) );
}

// ”множение вектора на скал€р (возврат по значению)
template <typename T, typename R2>
Array<T, A_Mult<T, A_Scalar<T>, R2> >		// return value
operator*(T const& a, Array<T, R2> const& b){

		return Array<T, A_Mult<T, A_Scalar<T>, R2> >( A_Mult<T, A_Scalar<T>, R2>( A_Scalar<T>(a), b.rep()) );
}
