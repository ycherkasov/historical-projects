#include "direct_instance.h"

// В функции void show_direct_instance() (main.cpp)
// требуются экземпляры direct_instance<int> и <double>
// Инстанцируем их явно до определения класса
template class direct_instance<int>;
template class direct_instance<double>;

// Можно инстанцировать как весь класс, так и отдельную функцию
// Если класс уже инстанцирован, функцию инстанцировать нельзя

template <typename T>
direct_instance<T>::direct_instance(const T& t) : _x(t) {}

template <typename T>
direct_instance<T>::~direct_instance(void){}

template <typename T>
T direct_instance<T>::test() const {
	return _x;
}
