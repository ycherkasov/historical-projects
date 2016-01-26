#include "StdAfx.h"
#include "PeriodicFunction.h"
#include "SerieSettings.h"

PeriodicFunction::PeriodicFunction(void)
	: m_values(SerieSettings::SerieSize())
{
}

PeriodicFunction::~PeriodicFunction(void)
{
}

point_t PeriodicFunction::GetValue(size_t i)
{
	return m_values[i];
}

void PeriodicFunction::AddHarmonic(const std::vector<double>& vals)
{
	try
	{
		for(int i = 0 ; i < SerieSettings::SerieSize(); i++)
			m_values.at(i).second += vals.at(i);
	}
	catch (const std::exception& e)
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
		return;
	}
}

void PeriodicFunction::ProduseHarmonic(const std::vector<double>& vals)
{
	try
	{
		for(int i = 0 ; i < SerieSettings::SerieSize(); i++)
			m_values.at(i).second *= vals.at(i);
	}
	catch (const std::exception& e)
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
		return;
	}
}

void PeriodicFunction::ResetWithSet(const ValuesSet& vals)
{
	try
	{
		m_values = vals;
	}
	catch (const std::exception& e)
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
		return;
	}
}

void PeriodicFunction::Clear()
{
	m_values.clear();
}