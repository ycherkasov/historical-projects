#include "intset_t.h"
#include <algorithm>
#include <iterator>

intset_t::intset_t()
{
}

intset_t::intset_t(const intset_t& i) : _set(i._set)
{
}

intset_t::intset_t(int* arr, size_t cnt)
{
	_set.assign(arr, arr + cnt);
	sort( _set.begin(), _set.end() );
}

intset_t::intset_t(const vector<int>& i)
{
	_set.assign( i.begin(), i.end() );
	sort( _set.begin(), _set.end() );
}

intset_t::~intset_t()
{
}

intset_t& intset_t::operator=(const intset_t& i)
{
	if( &i != this )
	{
		_set = i._set;
	}
	return *this;
}

intset_t& intset_t::operator+(const intset_t& i)
{
	vector<int> tmp;
	_set.insert( _set.end(), i._set.begin(), i._set.end() );
	sort( _set.begin(), _set.end() );
	unique_copy(  _set.begin(), _set.end(), back_inserter(tmp) );
	_set.swap(tmp);
	return *this;
}

intset_t& intset_t::operator-(const intset_t& i)
{
	vector<int>::const_iterator it = i._set.begin();
	while( it != i._set.end() ){

		// »щем в вычитаемом множестве совпадени€
		vector<int>::iterator it_remove = find( _set.begin(), _set.end(), (*it) );
		if( it_remove != _set.end() )
			_set.erase( it_remove );
		++it;
	}
	return *this;
}

intset_t& intset_t::operator*(const intset_t& i)
{
	vector<int> tmp;
	vector<int>::const_iterator it = i._set.begin();
	while( it != i._set.end() ){

		
		// »щем в вычитаемом множестве совпадени€
		vector<int>::const_iterator it_insert = find( _set.begin(), _set.end(), (*it) );
		if( it_insert != _set.end() )
			tmp.push_back( (*it_insert) );
		++it;
	}
	_set.swap(tmp);

	return *this;
}