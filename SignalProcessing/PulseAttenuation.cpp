#include "StdAfx.h"
#include "PulseAttenuation.h"
#include "PeriodicFunction.h"
#include "SerieSettings.h"

#include <cmath>
#include <process.h>

using namespace std;

vector<double> PulseAttenuation::_attenuation_func;
vector<double> PulseAttenuation::_custom_attenuation_func;

UINT PulseAttenuation::Folding(void* pthis)
{
	ValuesSet tmpValues;

	PeriodicFunction* _master_function = (PeriodicFunction*)pthis;
	HWND hwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	if(!hwnd)
		return -1;

	::SendMessage(hwnd, WM_START_RECOUNT, SerieSettings::SerieSize(), 0);

	_master_function->ProduseHarmonic(_attenuation_func);

	for(int i = 0; i < SerieSettings::SerieSize() ; i++)
	{
		double tmp = 0.;
		for(int t = 0 ; t < SerieSettings::SerieSize() ; t++)
		{
			if((i-t) >= 0)
			{
				tmp += _custom_attenuation_func.at(t)*_master_function->GetValue(i-t).second;
			}
			else
				continue;
		}
		tmpValues.push_back( make_pair( i*SerieSettings::StepVal(), tmp ) );
		::SendMessage(hwnd, WM_HOP_RECOUNT, i, 0);
	}
	_master_function->ResetWithSet(tmpValues);
	
	::SendMessage(hwnd, WM_END_RECOUNT, 0, 0);
	return 0;
}

void PulseAttenuation::GeterateAttenuationSerie(PeriodicFunction* master_func, const pulse_points_t& att_settings)
{
	double x_exp = 0.;
	for( int i = 0 ; i < SerieSettings::SerieSize() ; i++ )
	{
		double tmp = SerieSettings::StepVal();
		x_exp = exp(-i*tmp*10.);
		_attenuation_func.push_back(x_exp);

		std::map<int, int>::const_iterator it = att_settings.begin();
		if( (it = att_settings.find(i)) != att_settings.end() )
			_custom_attenuation_func.push_back( (*it).second );
		else
			_custom_attenuation_func.push_back( 0. );
	}
	UINT addr = -1;
	_beginthreadex(NULL, NULL, PulseAttenuation::Folding, master_func, NULL, &addr);
}