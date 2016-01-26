#pragma once

#include <valarray>

// Example Straus 3 p.738
template <typename T>
class slice_iter{
public:

	slice_iter(std::valarray<T>& v, std::slice s) :
		_vector(v),
		_slice(s),
		_curr(){}

	slice_iter end(){
		slice_iter end_slice = *this;
		end_slice._curr = _slice.size();
		return end_slice;
	}

	slice_iter& operator++(){
		++_curr;
		return *this;
	}

	slice_iter operator++(int){
		slice_iter t = *this;
		++_curr;
		return t;
	}

	// [] indexing
	T& operator[](size_t i){ return ref(_curr = i); }

	T& operator()(size_t i){ return ref(_curr = i); }

	T& operator*(){ return ref(_curr); }


private:
	// vector we iterate over
	std::valarray<T>& _vector;
	
	std::slice _slice;
	
	// current element index
	size_t _curr;

	T& ref(size_t s) const{
		return _vector[_slice.start() + s * _slice.stride()];
	}

public:
	// binary operators
	friend bool operator == (const slice_iter& v1, const slice_iter& v2){
		return v1._curr == v2._curr
			&& v1._slice.stride() == v2._slice.stride()
			&& v1._slice.start() == v2._slice.start();
	}

	friend bool operator != (const slice_iter& v1, const slice_iter& v2){
		return !(v1 == v2);
	}

	friend bool operator < (const slice_iter& v1, const slice_iter& v2){
		return v1._curr < v2._curr
			&& v1._slice.stride() == v2._slice.stride()
			&& v1._slice.start() == v2._slice.start();
	}

};


