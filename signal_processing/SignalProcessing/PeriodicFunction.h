#pragma once
#include "HarmonicsComposition.h"

class PeriodicFunction
{
private:
	ValuesSet m_values;
public:
	PeriodicFunction();
	~PeriodicFunction();

	ValuesSet& GetValuesSet(){ return m_values; }
	point_t GetValue(size_t i);
	void AddHarmonic(const std::vector<double>& vals);
	void ProduseHarmonic(const std::vector<double>& vals);
	void ResetWithSet(const ValuesSet& vals);

	void Clear();
	size_t Size(){return m_values.size();}
};
