#include <limits>
#include <iostream>
#include "tests.h"
using std::numeric_limits;
using std::cout;
using std::endl;


// ��������� ���������� ������������
template <typename T>
class range_error_t{
	
public:
	// ����������� �� ������� ����������
	template <typename T > range_error_t(T r) throw() { 
		_range = r;
	}

	// ����������� ����������� ���������� ������ ���� ������ �� ��������
	template <typename T > range_error_t(const range_error_t& r)  throw()  { // �� ������� ����������
		_range = r._range;
	}

	T range() const {return _range;} 

private:
	T _range;
};

// ������� ����������� ������������ ��� ��������� ���� ��� ��������
template <typename T>
T plus(T a, T b){
	if( ( numeric_limits<T>::max() < (a + b) ) || ( numeric_limits<T>::min() > (a + b) ) )
	{
		throw range_error_t<T>(a + b);
	}
	return a + b;
}

// ������� ����������� ������������ ��� ��������� ���� ��� ���������
template <typename T>
T minus(T a, T b){
	if( ( numeric_limits<T>::max() < (a - b) ) || ( numeric_limits<T>::min() > (a - b) ) )
	{
		throw range_error_t<T>(a - b);
	}
	return a - b;
}

void test_some_overflow(){
	short a = 30000;
	short b = 30000;
	try{
		plus(a, b);
	}
	catch( const range_error_t<short>& e ){
		std::cout << "Unable perform action, overflow with value: " << e.range() << std::endl;
	}
}
