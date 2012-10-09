#pragma once

// ��������� ����� ��� ������������� �������� ������
// ����� ����������� ����� ��� ���������� ���������� ������������ ��������
// �������� ������ ����������� � ��������� ���������� Blitz++

// ��������� ������
template <int DIM, typename T>
struct DotProduct{
	static T result(T* a, T* b){
		return (*a) * (*b) + DotProduct<DIM-1, T>::result(a+1, b+1);
	}
};


// ��������� ������������� ��������� ��������
template <typename T>
struct DotProduct<1, T>{
	static T result(T* a, T* b){
		return (*a) * (*b);
	}
};

// ������� ���������� ������������
template <int DIM, typename T>
inline T dot_product(T* a, T* b){
	return DotProduct<DIM, T>::result(a, b);
}