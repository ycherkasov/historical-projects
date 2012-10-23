#pragma once
#include "simple_array.h"
#include "expressions.h"

template <typename T, typename Rep = SArray<T> >
class Array{
private:
	// Инкапсуляция данных (представление массива)
	Rep expr_rep;
public:

	// Массив указанного размера
	explicit Array(size_t s): expr_rep(s){}

	// Массив из существующего представления
	explicit Array(Rep const& r): expr_rep(r){}

	// Оператор присвоения для тогоже типа
	Array& operator=(Array const& rhs){

		print_debug("=");
		assert(size() == rhs.size());
		for(size_t i = 0; i < size(); ++i){
			expr_rep[i] = rhs[i];
		}
		return *this;
	}

	// Оператор присвоения для другого типа
	template <typename T2, typename Rep2>
	Array& operator=(Array<T2, Rep2> const& rhs){

		print_debug("=");
		assert(size() == rhs.size());
		for(size_t i = 0; i < size(); ++i){
			expr_rep[i] = rhs[i];
		}
		return *this;
	}

	// размер представления
	size_t size() const {return expr_rep.size();}

	// операторы индексирования
	T operator[](size_t i) const {return expr_rep[i];}

	T& operator[](size_t i) {return expr_rep[i];}

	Rep& rep() {return expr_rep;}
	Rep const& rep() const {return expr_rep;}

};

// Реализуем перегруженные функции векторной арифметики
// теперь благодаря шаблонам выражений часть вычислений будет свернута


// Сложение векторов (возврат по значению)
template <typename T, typename R1, typename R2>
Array<T, A_Add<T, R1, R2> >		// return value
operator+(Array<T, R1> const& a, Array<T, R2> const& b){

	return Array<T, A_Add<T, R1, R2> >( A_Add<T, R1, R2>(a.rep(), b.rep()) );
}

// Умножение векторов (возврат по значению)
template <typename T, typename R1, typename R2>
Array<T, A_Mult<T, R1, R2> >		// return value
operator*(Array<T, R1> const& a, Array<T, R2> const& b){

		return Array<T, A_Mult<T, R1, R2> >( A_Mult<T, R1, R2>(a.rep(), b.rep()) );
}

// Умножение вектора на скаляр (возврат по значению)
template <typename T, typename R2>
Array<T, A_Mult<T, A_Scalar<T>, R2> >		// return value
operator*(T const& a, Array<T, R2> const& b){

		return Array<T, A_Mult<T, A_Scalar<T>, R2> >( A_Mult<T, A_Scalar<T>, R2>( A_Scalar<T>(a), b.rep()) );
}
