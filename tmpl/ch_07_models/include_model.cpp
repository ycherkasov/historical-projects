#include "include_model.h"

// «десь нельз€ определ€ть методы шаблонного класса
// потому что при его компил€ции еще неизвестно,
// какого типа экземпл€р шаблона надо инстанцировать

#if 0
template <typename T>
include_model<T>::include_model(const T& t) : _x(t){}

template <typename T>
include_model<T>::~include_model(void){}

template <typename T>
T include_model<T>::test() const {
	return _x;
}
#endif
