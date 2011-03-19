#pragma once

// ------------------- Модель включения -------------------

template <typename T>
class include_model
{
public:
	include_model(const T& t);
	~include_model();
	T test() const;
private:
	T _x;
};

// Определение нельзя вынести в *cpp, потому что при его компиляции еще неизвестно,
// какого типа экземпляр шаблона надо инстанцировать

// Как результат - используется "модель включения",
// заключающаяся в определении шаблонного класса и методов в хедере

// Это порождает 2 проблемы:
// 1. Увеличение включенных хедеров (ведь все стандартные хедеры тоже приходится 
// включать в хедер с шаблоном)
// 2. На некоторых компиляторах шаблонные инлайн-функции определяются 
// как нарушения правила ODR, хотя это и не проблема языка.


// Определение:
template <typename T>
include_model<T>::include_model(const T& t) : _x(t){}

template <typename T>
include_model<T>::~include_model(void){}

template <typename T>
T include_model<T>::test() const {
	return _x;
}