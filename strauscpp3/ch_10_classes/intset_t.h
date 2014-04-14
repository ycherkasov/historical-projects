#pragma once
#include <vector>
using std::vector;

// Класс для хранения множеств
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

	// объединение множеств
	intset_t& operator+(const intset_t& i);

	// пересечение множеств
	intset_t& operator*(const intset_t& i);

	// симметрическая разность
	intset_t& operator-(const intset_t& i);
};
