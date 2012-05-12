#include "stdafx.h"
#include "PeriodicFunction.h"
#include "ChartDemo2Dlg.h"
#define _USE_MATH_DEFINES
#include <cmath>


CPeriodicFunction::CPeriodicFunction(size_t sz, int a0, int f0, double dt) :
	m_a0(a0), m_f0(f0), m_dt(dt)
{
	m_values.reserve(sz);
	Count();
}

CPeriodicFunction::~CPeriodicFunction()
{
}


void CPeriodicFunction::Count(void)
{
	
	for (double t=0 ; t < (SERIE_SIZE*m_dt) ; t += m_dt)
	{
		double Y = (m_a0) * sin(2*M_PI*(m_f0)*t);;
		m_values.push_back( std::make_pair(t,Y) );
	}
}


bool CPeriodicFunction::GetPair(int index, double& x, double& y)
{
	try
	{
		x = m_values.at(index).first;
		y = m_values.at(index).second;
	}
	catch (std::exception* e) 
	{
		::MessageBoxA(NULL, e->what(), "Exception", MB_OK);
		return false;
	}
	return true;
}