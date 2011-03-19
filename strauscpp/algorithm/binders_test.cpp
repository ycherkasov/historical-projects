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

	int i;
	for (i = 0; i <= 5; i++)	{
		v1.push_back(5 * i);
	}

	cout << "The vector v1 = ( " ;
	for (Iter = v1.begin(); Iter != v1.end(); Iter++)
		cout << *Iter << " ";
	cout << ")" << endl;

	// Count the number of integers > 10 in the vector
	vector<int>::iterator::difference_type result1a
		 = count_if(v1.begin(), v1.end(), bind1st(less<int>(), 10));

	cout << "The number of elements in v1 greater than 10 is: "
		<< result1a << "." << endl;

	// Compare: counting the number of integers > 15 in the vector
	// with a user defined function object
	vector<int>::iterator::difference_type result1b
		 = count_if(v1.begin(), v1.end(), greaterthan5());
	
	cout << "The number of elements in v1 greater than 15 is: "
		<< result1b << "." << endl;

	// Count the number of integers < 10 in the vector
	vector<int>::iterator::difference_type result2
		= count_if(v1.begin(), v1.end(), bind2nd(less<int>(), 10));
	
	cout << "The number of elements in v1 less than 10 is: "
		<< result2 << "." << endl;
}