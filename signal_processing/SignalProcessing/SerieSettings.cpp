#include "StdAfx.h"
#include "SerieSettings.h"

int SerieSettings::serie_size = 1000;
double SerieSettings::step_val = 0.001;

void SerieSettings::SetSerieSize(int new_size)
{
	serie_size = new_size; 
	step_val = 1./static_cast<double>(new_size);
}
