#pragma once
#include <vector>

class controlled
{
public:
	controlled();
	~controlled() throw();
	void controlled_method() const throw(std::exception);
private:
	std::vector<int> _vect;
};
