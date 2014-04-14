#pragma once
#include <vector>
using std::vector;

// ����� ��� �������� ��������
class intset_t
{
	vector<int> _set;
public:
	intset_t();
	intset_t(int* arr, size_t cnt);
	intset_t(const vector<int>& i);
	intset_t(const intset_t& i);
	~intset_t();


	intset_t& operator=(const intset_t& i);

	// ����������� ��������
	intset_t& operator+(const intset_t& i);

	// ����������� ��������
	intset_t& operator*(const intset_t& i);

	// �������������� ��������
	intset_t& operator-(const intset_t& i);
};
