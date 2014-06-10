#pragma once

#include <valarray>
#include "slice_iter.h"

template <typename T>
class matrix{
public:

	matrix(size_t d1, size_t d2) :_d1(d1), _d2(d2), _val(d1*d2){}

	matrix(const matrix& rhs) :_d1(rhs._d1), _d2(rhs._d2), _val(rhs._val){}

	const matrix& operator=(const matrix& rhs){
		_d1 = rhs._d1;
		_d2 = rhs._d2;
		_val = rhs._val;
	}

	// non-virtual destructor
	~matrix(){}

	// accessors
	size_t size() const { return _d1*_d2; }
	size_t dim1() const { return _d1; }
	size_t dim2() const { return _d2; }

	// row-column iterators
	slice_iter<T> row(size_t i){
		return slice_iter<T>(_val, slice(i, _d2, _d1));
	}

	slice_iter<T> column(size_t i){
		return slice_iter<T>(_val, slice(i*_d2, _d2, 1));
	}

	const slice_iter<T> row(size_t i) const {
		return slice_iter<T>(_val, slice(i, _d2, _d1));
	}

	const slice_iter<T> column(size_t i) const {
		return slice_iter<T>(_val, slice(i*_d2, _d2, 1));
	}

	// access elements
	T& operator()(size_t x, size_t y) {
		return row(x)[y];
	}

	T operator()(size_t x, size_t y) const {
		return row(x)[y];
	}

private:
	std::valarray<T> _val;
	size_t _d1;
	size_t _d2;
};
