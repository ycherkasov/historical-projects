#pragma once

// �������� ������ ��� ���������� ����� N � ������� POW
template <int N, int POW>
struct Pow{
	enum {result = N * Pow<N, POW-1>::result };
};

// ��������� ������������� ��� ���������� ��������
template <int N>
struct Pow<N, 0>{
	enum {result = 1 };
};
