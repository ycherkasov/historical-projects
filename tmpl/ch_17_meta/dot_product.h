#pragma once

// Ўаблонный класс дл€ развертывани€ коротких циклов
// «десь представлен класс дл€ нахождени€ скал€рного произведени€ массивов
// ѕодобные приемы примен€ютс€ в численной библиотеке Blitz++

// ѕервичный шаблон
template <int DIM, typename T>
struct DotProduct{
	static T result(T* a, T* b){
		return (*a) * (*b) + DotProduct<DIM-1, T>::result(a+1, b+1);
	}
};


// „астична€ специализаци€ окончани€ рекурсии
template <typename T>
struct DotProduct<1, T>{
	static T result(T* a, T* b){
		return (*a) * (*b);
	}
};

// ‘ункци€ скал€рного произведени€
template <int DIM, typename T>
inline T dot_product(T* a, T* b){
	return DotProduct<DIM, T>::result(a, b);
}