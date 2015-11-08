#pragma once
#include "HarmonicsComposition.h"

class HarmonicFile
{
public:
	HarmonicFile(void);
	~HarmonicFile(void);
	static void FromFile(LPCTSTR fname, ValuesSet& vals);
	static void ToFile(LPCTSTR fname, const ValuesSet& vals);
};
