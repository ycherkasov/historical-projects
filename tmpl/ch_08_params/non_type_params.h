#pragma once

// ������������ ��������� ����������, �� ���������� ������
// 1. ������������� ���, � �.�. enum
// 2. ��������� � ������� ����������� ()
// 3. ������ � ������� �����������

// �� �.�. static � mutable, ��������� const � volatile ������������
// ������ �������� rvalue

// ------------------- ������������� ��� -------------------
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

// ------------------- ��������� -------------------
// ����� ����� ������������ �������� ����� typename,
// ���� � ��������� ������������ ���, ��������
template <typename T, typename T::allocator* ALLOC>
class List;

// ------------------- ������ -------------------
template <typename T, int& MAX>
class Deque;