#include "StdAfx.h"
#include "NoiseGenerator.h"
#include "SerieSettings.h"

float NoiseGenerator::FRand(float ran)
{
	static unsigned long iran = ran*1000;
	unsigned long temp;
	float fran;

	static unsigned long jflone=0x3f800000;
	static unsigned long jflmsk=0x007fffff;

	iran=1664525L*iran+1013904223L;
	temp=jflone|(jflmsk&iran);
	return (*(float *)&temp)-1.F;
}

void NoiseGenerator::GenerateWhiteNoise(std::vector<double>& out_serie)
{
	srand( static_cast<unsigned>( time(0) ) );
	out_serie.clear();
	double RANGE_MIN = -10.;
	double RANGE_MAX = 10.;
	double rand_one = rand();
	for( int i = 0 ; i < SerieSettings::SerieSize() ; i++ )
	{
		rand_one = FRand(rand_one);
		double rand100 = rand_one * RANGE_MAX + RANGE_MIN;
		out_serie.push_back(rand100);
	}
	UINT addr = -1;
}

void NoiseGenerator::GenerateJumpNoise(std::vector<double>& out_serie)
{
	std::vector<double> x_points;
	std::vector<double> jump_values;

	srand( static_cast<unsigned>( time(0) ) );
	double rand_one = rand();
	
	double range_value_min = 1.;
	double range_value_max = 100.;

	for( int i = 0 ; i < SerieSettings::SerieSize() ; i++ )
	{
		rand_one = FRand(rand_one);
		if( rand_one <= 0.01 )
		{
			rand_one = FRand(rand_one);
			double r = rand_one * range_value_max + range_value_min;
			out_serie.push_back(r);
		}
		else
			out_serie.push_back(0.);
	}
}

void NoiseGenerator::GenerateShiftNoise(std::vector<double>& out_serie)
{
	srand( static_cast<unsigned>( time(0) ) );
	double rand_one = rand();

	double range_value_min = 1000.;
	double range_value_max = 10000.;
	rand_one = FRand(rand_one);
	double r = rand_one * range_value_max + range_value_min;
	out_serie.assign(SerieSettings::SerieSize(),r);
}