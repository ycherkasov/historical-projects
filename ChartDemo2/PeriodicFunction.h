#pragma once
#include <vector>

class CPeriodicFunction
{
public:
	std::vector< std::pair<double,double> > m_values;
	int m_a0;	// 10..50
	int m_f0;	// 5..500
	double m_dt;	// 0.001..0.1

	CPeriodicFunction(size_t sz, int a0, int f0, double dt);
	~CPeriodicFunction();
	void Count();
	bool GetPair(int index, double& x, double& y);
};
