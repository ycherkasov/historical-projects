#include <exception>

// Глава 4 Джосаттиса
// Параметры шаблонов, не являющиеся типами

// Шаблон массива с проверкой границ
// обнуляется на запросу и бросает исключения
// Кстати, можно задать размер по умолчанию,
// но в данном случае это будет bad design
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

// Подобные параметры можно использовать и в функциях
// Пример, немного надуманный, но
template <typename T, int VAL>
T add_value(const T& t){
	return t + VAL;
}

// Нетиповые параметры шаблонов могут быть
// - целочисленными константами
// - указателями на объекты с внешним связыванием