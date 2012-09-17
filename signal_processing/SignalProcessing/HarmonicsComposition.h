#pragma once
#include <vector>
#include <list>

struct ListHarmonicData
{
	ListHarmonicData(int a, int f) : a0(a), f0(f){}
	int a0;
	int f0;
};

typedef std::list<ListHarmonicData*> FunctionsSet;

typedef std::pair< double, double > point_t;
typedef std::vector< point_t > ValuesSet;

class HarmonicsComposition
{
public:
	static void Compose(FunctionsSet& fs, ValuesSet& vs);
};
