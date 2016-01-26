#pragma once

// ƒемонстраци€ шаблонных параметров, не €вл€ющихс€ типами
// 1. ÷елочисленный тип, в т.ч. enum
// 2. ”казатели с внешним св€зыванием ()
// 3. —сылки с внешним св€зыванием

// не м.б. static и mutable, параметры const и volatile игнорируютс€
// ¬сегда €вл€ютс€ rvalue

// ------------------- ÷елочисленный тип -------------------
template <typename T, int MAX>
class fixed_array{
public:
	fixed_array();
	T& operator[](int index);
	const T& operator[](int index) const;
private:
	T _arr[MAX];
};

template <typename T, int MAX>
fixed_array<T,MAX>::fixed_array() : _arr(){}

template <typename T, int MAX>
T& fixed_array<T,MAX>::operator [](int index) {
	if( (index < 0) || (index >= MAX) ) 
		throw std::out_of_range("Array ranges error");
	return _arr[index];
}

template <typename T, int MAX>
const T& fixed_array<T,MAX>::operator [](int index) const {
	if( (index < 0) || (index >= MAX) ) 
		throw std::out_of_range("Array ranges error");
	return _arr[index];
}

// ------------------- ”казатели -------------------
// «десь может фигурировать ключевое слово typename,
// если в параметре используетс€ тип, например
template <typename T, typename T::allocator* ALLOC>
class List;

// ------------------- —сылки -------------------
template <typename T, int& MAX>
class Deque;