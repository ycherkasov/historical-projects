#include "adapters.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

using namespace std;

// Creation of a user-defined function object
// that inherits from the unary_function base class
class greaterthan5: unary_function<int, bool>
{
public:
	result_type operator()(argument_type i)
	{
		return (result_type)(i > 5);
	}
};

void binders_test()
{
	vector<int> v1;
	vector<int>::iterator Iter;

	for (int i = 0; i <= 5; i++)	{
		v1.push_back(5 * i);
	}

	cout << "The vector v1 = ( " ;
	for (Iter = v1.begin(); Iter != v1.end(); Iter++)
		cout << *Iter << " ";
	cout << ")" << endl;

	// Алгороитмы могут возвращать разные типы, какие - описано в iterator_traits
	// (для защиты от переполнения)

	// Количество целых > 10
	vector<int>::iterator::difference_type result1a
		 = count_if(v1.begin(), v1.end(), bind1st(less<int>(), 10));

	cout << "The number of elements in v1 less than 10 is: "
		<< result1a << "." << endl;

	// Количество целых > 5
	vector<int>::iterator::difference_type result1b
		 = count_if(v1.begin(), v1.end(), greaterthan5());
	
	cout << "The number of elements in v1 greater than 5 is: "
		<< result1b << "." << endl;

	// Количество целых < 10
	vector<int>::iterator::difference_type result2
		= count_if(v1.begin(), v1.end(), bind2nd(greater<int>(), 10));
	
	cout << "The number of elements in v1 greater than 10 is: "
		<< result2 << "." << endl;
}