#pragma once
#include "recursive_pair.h"

// Можно наследовать от рекурсивного кортежа шаблон со списком типов, 
// для удобства записи
struct NullType{};

// Общий рекурсивный шаблон для кортежа
template <
	typename T1, 
	typename T2 = NullType,
	typename T3 = NullType,
	typename T4 = NullType,
	typename T5 = NullType
>
struct Tuple : public Pair<T1, typename Tuple<T2,T3,T4,T5,NullType>::BaseType >{
	
	typedef Pair<T1, typename Tuple<T2,T3,T4,T5,NullType>::BaseType > BaseType;

	Tuple(){}

	Tuple(T1 const& a1, 
		T2 const& a2, 
		T3 const& a3 = NullType(), 
		T4 const& a4 = NullType(), 
		T5 const& a5 = NullType())
		:
	BaseType(a1, Tuple<T2,T3,T4,T5,NullType>(a2,a3,a4,a5))
	{}
};

// Специализация для завершения рекурсии
template <typename T1, typename T2>
struct Tuple<T1,T2> : public Pair<T1, T2> {
	
	typedef Pair<T1, T2> BaseType;

	Tuple(){}

	Tuple(T1 const& a1, 
		T2 const& a2, 
		NullType const& = NullType(), 
		NullType const& = NullType(), 
		NullType const& = NullType())
		:
	BaseType(a1, a2)
	{}
};

// Специализация для объекта с одним полем
template <typename T1>
struct Tuple<T1,NullType,NullType,NullType,NullType> : public Pair<T1, void> {

	typedef Pair<T1, void> BaseType;

	Tuple(){}

	Tuple(T1 const& a1, 
		NullType const& = NullType(),
		NullType const& = NullType(), 
		NullType const& = NullType(), 
		NullType const& = NullType())
		:
	BaseType(a1)
	{}
};

// Функции для конструирования кортежей
template <typename T1>
inline Tuple<T1> make_tuple(T1 const& t1){
	return Tuple<T1>(t1);
}

template <typename T1, typename T2>
inline Tuple<T1,T2> make_tuple(T1 const& t1, T2 const& t2){
	return Tuple<T1,T2>(t1,t2);
}

template <typename T1, typename T2, typename T3>
inline Tuple<T1,T2,T3> make_tuple(T1 const& t1, T2 const& t2, T3 const& t3){
	return Tuple<T1,T2,T3>(t1,t2,t3);
}

template <typename T1, typename T2, typename T3, typename T4>
inline Tuple<T1,T2,T3,T4> make_tuple(T1 const& t1, T2 const& t2, T3 const& t3, T4 const& t4){
	return Tuple<T1,T2,T3,T4>(t1,t2,t3,t4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline Tuple<T1,T2,T3,T4,T5> make_tuple(T1 const& t1, T2 const& t2, T3 const& t3, T4 const& t4, T5 const& t5){
	return Tuple<T1,T2,T3,T4,T5>(t1,t2,t3,t4,t5);
}
