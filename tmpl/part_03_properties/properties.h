#pragma once

// Глава 15.1 Джосаттиса
// Стратегии и свойства

// ------------------- Простой аккумулятор -------------------
// Для типов, поддерживающих суммирующее аккумулирование
template <typename T>
inline T simple_accumulate(const T* begin, const T* end){
	
	// Накопитель с инициализацией
	T total = T();

	// Итерирование
	while( begin != end ){
		total += *begin;
		++begin;
	}
	return total;
}
// Недостаток в том, что возвращаемый тип не защищен от переполнения
// Например в случае char (byte)

// ------------------- Фиксированные классы свойств -------------------
// Выход в явном задании возвращаемого типа в зависимости от суммируемого
// Вводим шаблон свойств (traits), который определяет возвращаемый тип в зависимости от переданного
template <typename T>
class simple_accumulator_traits{};

// Сами типы определим с помощью частичной специализации
template <>
class simple_accumulator_traits<char>{
public:
	typedef int AccType;
};

template <>
class simple_accumulator_traits<short>{
public:
	typedef int AccType;
};


template <>
class simple_accumulator_traits<int>{
public:
	typedef long AccType;
};

template <>
class simple_accumulator_traits<unsigned int>{
public:
	typedef unsigned long AccType;
};

template <>
class simple_accumulator_traits<float>{
public:
	typedef double AccType;
};

// Тогда функция примет вид
template <typename T> inline typename 
simple_accumulator_traits<T>::AccType traits_accumulate(const T* begin, const T* end){

	// Накопитель с инициализацией
	typedef typename simple_accumulator_traits<T>::AccType AccT;
	AccT total = AccT();

	// Итерирование
	while( begin != end ){
		total += *begin;
		++begin;
	}
	return total;
}


// ------------------- Свойства-значения -------------------
// Кроме типов, traits могут передавать также значения
// Например, нулевое значение, которое выполняется конструктором по умолчанию
// может быть изменено и передано с помощью traits
template <typename T>
class value_accumulator_traits;

template <>
class value_accumulator_traits<char>{
public:
	typedef int AccType;
	static AccType zero(){ return 0; }
};

template <>
class value_accumulator_traits<short>{
public:
	typedef int AccType;
	static AccType zero(){ return 0; }
};


template <>
class value_accumulator_traits<int>{
public:
	typedef long AccType;
	static AccType zero(){ return 0l; }
};

template <>
class value_accumulator_traits<unsigned int>{
public:
	typedef unsigned long AccType;
	static AccType zero(){ return 0u; }
};

template <>
class value_accumulator_traits<float>{
public:
	typedef double AccType;
	static AccType zero(){ return 0.; }
};

// Тогда функция примет вид
template <typename T> inline typename 
value_accumulator_traits<T>::AccType traits_vals_accumulate(const T* begin, const T* end){

	// Накопитель с инициализацией
	typedef typename value_accumulator_traits<T>::AccType AccT;
	AccT total = value_accumulator_traits<T>::zero();

	// Итерирование
	while( begin != end ){
		total += *begin;
		++begin;
	}
	return total;
}

// ------------------- Параметризованные свойства  -------------------
// Приведенные выше свойства называются фиксированными, т.к. их нельзя заменить в алгоритме
// Решается добавлением в алгоритм параметра шаблона со значением по умолчанию
// (т.к. в большинстве случаев стандартные свойства удовлетворяют его)
// Тогда функцию придется заменить на класс, т.к. функции не поддерживают значения 
// шаблонного параметра по умолчанию
template <typename T, typename AccTraits = value_accumulator_traits<T> >
class custom_accumulator_traits{
public:
	static typename AccTraits::AccType accum(const T* begin, const T* end){
		// Накопитель с инициализацией
		typename AccTraits::AccType total = AccTraits::zero();

		// Итерирование
		while( begin != end ){
			total += *begin;
			++begin;
		}
		return total;
	}
};

// ------------------- Стратегии -------------------
// Очевидно, аккумулирование может быть не только суммированием
// Можно, например, аккумултровать умножением. Для этого введем понятие 
// стратегии аккумулирования (accumulation policy)
class SumPolicy{
public:
	template <typename T1, typename T2>
	static void accumulate(T1& total, const T2& value){
		total += value;
	}
};

class MultPolicy{
public:
	template <typename T1, typename T2>
	static void accumulate(T1& total, const T2& value){
		total *= value;
	}
};

template <typename T
, typename Policy = SumPolicy
, typename AccTraits = value_accumulator_traits<T> >
class strategy_accumulator_traits{
public:
	static typename AccTraits::AccType accum(const T* begin, const T* end){
		// Накопитель с инициализацией
		typename AccTraits::AccType total = AccTraits::zero();

		// Итерирование
		while( begin != end ){
			Policy::accumulate(total, *begin);
			++begin;
		}
		return total;
	}
};

// ------------------- Стратегии с шаблонными аргументами -------------------
// Можно заменить классы стратегий на шаблоны
// И добавим третий параметр в метод для начального элемента
template <typename T1, typename T2>
class SumPolicyT{
public:
	static void accumulate(T1& total, const T2& value){
		total += value;
	}
};

template <typename T1, typename T2>
class MultPolicyT{
public:
	static void accumulate(T1& total, const T2& value){
		total *= value;
	}
};

template <typename T
, template<typename,typename> class Policy = SumPolicyT
, typename AccTraits = value_accumulator_traits<T> >
class strategy_tmpl_accumulator_traits{
public:
	static typename AccTraits::AccType accum(const T* begin, const T* end
		,typename AccTraits::AccType total_1st = AccTraits::zero() ){
		
			// Накопитель с инициализацией
			typedef AccTraits::AccType AT;
			AT total = total_1st;

			// Итерирование
			while( begin != end ){
				Policy<AT,T>::accumulate(total, *begin);
				++begin;
			}
			return total;
	}
};


