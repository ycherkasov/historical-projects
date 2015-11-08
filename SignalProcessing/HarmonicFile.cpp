#include "StdAfx.h"
#include "HarmonicFile.h"
#include "SerieSettings.h"

#include <fstream>
using namespace std;

HarmonicFile::HarmonicFile(void)
{
}

HarmonicFile::~HarmonicFile(void)
{
}

void HarmonicFile::FromFile(LPCTSTR fname, ValuesSet& vals)
{
	// поток ввода связать с файлом источника
	try
	{
		basic_ifstream<short> inputFile( fname, ios::in | ios::binary );
		vector<short> filedata( ( istreambuf_iterator<short>( inputFile ) ),  istreambuf_iterator<short>() );
		double x = 0.;
		vector<short>::const_iterator it = filedata.begin();
		while(it != filedata.end())
		{
			vals.push_back( make_pair(x, static_cast<double>(*it)) );
			x += SerieSettings::StepVal();
			++it;
		}		
	}
	catch (const std::exception& e)
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
		return;
	}
}

void HarmonicFile::ToFile(LPCTSTR fname, const ValuesSet& vals)
{
	vector<short> outData;
	ValuesSet::const_iterator it = vals.begin();
	while(it != vals.end())
	{
		outData.push_back( static_cast<short>((*it).second) );
		++it;
	}
	
	try 
	{
		basic_ofstream<short> ofs;
		ofs.exceptions(ios_base::failbit | ios_base::badbit);
		ofs.open(fname, ios_base::out | ios_base::trunc | ios_base::binary);
		ofs.write(&outData[0], outData.size());
	}
	catch(const std::exception& e) 
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
		return;
	} 
}
