#include "StdAfx.h"
#include "HarmonicsComposition.h"
#include "SerieSettings.h"

#define _USE_MATH_DEFINES
#include <cmath>



void HarmonicsComposition::Compose(FunctionsSet& fs, ValuesSet& vs)
{
	vs.clear();
	

	for (double t = 0 ; t < (SerieSettings::SerieSize()*SerieSettings::StepVal()) ; t += SerieSettings::StepVal())
	{
		double Y = 0.;
		FunctionsSet::iterator it = fs.begin();
		while( it != fs.end() )
		{
			Y += ( (*it)->a0 ) * sin(2*M_PI*( (*it)->f0 )*t);
			++it;
		}
		vs.push_back( std::make_pair(t,Y) );
	}
}