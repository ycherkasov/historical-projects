#pragma once

// Свойства шаблона элемента вычислений - передача по значению или по ссылке

template <typename T> class A_Scalar;

// Первичный шаблон - по умолчанию передача по ссылке (для векторов)
template <typename T>
struct A_Traits{
	typedef T const& ExprRef;
};


// Специализация для скаляра, передача по значению
template <typename T>
struct A_Traits< A_Scalar<T> >{
	typedef A_Scalar<T> ExprRef;
};

// Объект скаляра - в качестве "адаптера" для шаблонных выражений
// чтобы шаблоны видели его также как массив
template <typename T>
class A_Scalar{
private:
	T const& val;
public:
	A_Scalar(T const& v):val(v){}

	T operator[](size_t) const {return val;}
	T const& rep() const {return val;}
	size_t size() const {return 0;}
};

// Объекты для вычисления шаблонных выражений

// +
// Бинарная операция с отложенным вычислением
template <typename T, typename OP1, typename OP2>
class A_Add{
private:
	// Ссылки на операнды
	typename A_Traits<OP1>::ExprRef op1;
	typename A_Traits<OP2>::ExprRef op2;
public:

	// Конструктор - инициализация ссылок
	A_Add(OP1 const& a, OP2 const& b)
		:op1(a),op2(b) {}

	// "Отложенное" вычисление операции
	T operator[](size_t i) const {
		return op1[i] + op2[i];
	}

	size_t size() const {
		//assert(op2.size() == 0);
		return op2.size();
	}

};

// *
// Бинарная операция с отложенным вычислением
template <typename T, typename OP1, typename OP2>
class A_Mult{
private:
	// Ссылки на операнды
	typename A_Traits<OP1>::ExprRef op1;
	typename A_Traits<OP2>::ExprRef op2;
public:

	// Конструктор - инициализация ссылок
	A_Mult(OP1 const& a, OP2 const& b)
		:op1(a),op2(b) {}

	// "Отложенное" вычисление операции
	T operator[](size_t i) const {
		return op1[i] * op2[i];
	}

	size_t size() const {
		return op2.size();
	}

};