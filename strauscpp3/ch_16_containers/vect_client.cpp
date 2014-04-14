#include <iostream>
#include <vector>
#include "lvector.h"

using namespace std;

int test_user_vector() {
	
	lvector<int> v;
	size_t sz = sizeof(v);
	
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	sz = sizeof(v);

	lvector<int>::iterator it;
	it = v.begin();
	while(it!=v.end())
	{
		cout<<*it<<endl;
		++it;
	}

	vector<int> vv;
	sz = sizeof(vv);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	sz = sizeof(vv);
	
	return 0;
}