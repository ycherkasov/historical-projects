#include "direct_instance.h"

// � ������� void show_direct_instance() (main.cpp)
// ��������� ���������� direct_instance<int> � <double>
// ������������ �� ���� �� ����������� ������
template class direct_instance<int>;
template class direct_instance<double>;

// ����� �������������� ��� ���� �����, ��� � ��������� �������
// ���� ����� ��� �������������, ������� �������������� ������

template <typename T>
direct_instance<T>::direct_instance(const T& t) : _x(t) {}

template <typename T>
direct_instance<T>::~direct_instance(void){}

template <typename T>
T direct_instance<T>::test() const {
	return _x;
}
