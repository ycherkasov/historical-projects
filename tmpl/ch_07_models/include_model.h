#pragma once

// ------------------- ������ ��������� -------------------

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

// ����������� ������ ������� � *cpp, ������ ��� ��� ��� ���������� ��� ����������,
// ������ ���� ��������� ������� ���� ��������������

// ��� ��������� - ������������ "������ ���������",
// ������������� � ����������� ���������� ������ � ������� � ������

// ��� ��������� 2 ��������:
// 1. ���������� ���������� ������� (���� ��� ����������� ������ ���� ���������� 
// �������� � ����� � ��������)
// 2. �� ��������� ������������ ��������� ������-������� ������������ 
// ��� ��������� ������� ODR, ���� ��� � �� �������� �����.


// �����������:
template <typename T>
include_model<T>::include_model(const T& t) : _x(t){}

template <typename T>
include_model<T>::~include_model(void){}

template <typename T>
T include_model<T>::test() const {
	return _x;
}