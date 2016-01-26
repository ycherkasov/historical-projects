#include "StdAfx.h"
#include "SignalFilter.h"
#include "PeriodicFunction.h"
#include "SerieSettings.h"


#include <cmath>
#include <algorithm>
#include <process.h>
#include <iterator>


using namespace std;

double SignalFilter::_tmp_dev = 0.;
int SignalFilter::_weight_count = 0;
vector<double> SignalFilter::_folding_function;
vector<double> SignalFilter::_folding_function_down;
vector<double> SignalFilter::_folding_function_up;

UINT SignalFilter::Folding(void* pthis)
{
	ValuesSet tmpValues;
	PeriodicFunction* _master_function = (PeriodicFunction*)pthis;
	HWND hwnd = 0;
	if( !(hwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd()) )
		return -1;

	::SendMessage(hwnd, WM_START_RECOUNT, SerieSettings::SerieSize(), 0);

	for( int i = 0; i < _master_function->Size() ; i++ )
	{
		double tmp = 0.;
		for( int t = 0 ; t < _folding_function.size() ; t++ )
		{

			if( (i-t) >= 0 )
			{
				tmp += _folding_function.at(t)*_master_function->GetValue(i-t).second;
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

void SignalFilter::WeightFunctionCount(std::vector<double>& folding_output, unsigned frequency)
{
	folding_output.clear();
	double d[4] = { 0.35577019, 0.24369830, 0.07211497, 0.00630165 };

	if( !_weight_count )
		return;
	
	double fact = 2.0 * (double)frequency * SerieSettings::StepVal();
	folding_output.push_back(fact);
	fact = fact * M_PI;

	for( int k = 1; k < _weight_count; k++ )
		folding_output.push_back(sin(fact * k)/(M_PI * k));

	folding_output.at(_weight_count-1) /= 2.0;

	// TODO : transform
	// Potter window
	double sumg = folding_output.at(0);
	double sum = 0.;
	for(int k = 1; k<_weight_count; k++)
	{
		sum  = d[0];
		fact = M_PI * (double)(k/_weight_count);
		for(int l = 1; l <= 3; l++)
		{
			sum = sum + 2. * d[l] * cos(fact*l);
		}
		folding_output.at(k) *= sum;
		sumg = sumg + 2. * folding_output.at(k);
	}

	// TODO : transform	
	for(int k = 0; k<_weight_count; k++)
	{
		folding_output.at(k) /= sumg;
	}
	
}

void SignalFilter::WeightFunctionMirror(std::vector<double>& folding_output)
{
	vector<double> dbl_folding(folding_output.rbegin(), folding_output.rend());
	dbl_folding.insert(dbl_folding.end(), folding_output.begin(), folding_output.end());
	folding_output.swap( dbl_folding );
}

void SignalFilter::ApplyLowFrequencyFilter()
{
	_folding_function.clear();
	_folding_function.swap( _folding_function_up );
}

void SignalFilter::ApplyHighFrequencyFilter()
{
	_folding_function.clear();
	_folding_function.reserve( _folding_function_down.size() );
	_folding_function.push_back(1.0 - _folding_function_down[0]);

	struct negate{ double operator()(const double d){ return -d; } };	
	std::transform( _folding_function_down.begin() + 1, _folding_function_down.end()
		, std::back_inserter(_folding_function), negate() );
}

void SignalFilter::ApplyBandFilter()
{
	_folding_function.clear();
	// TODO : some algorithm
	_folding_function.reserve( _folding_function_down.size() );
	for(int k = 0 ; k < _folding_function_up.size() ; k++)
	{
		_folding_function.push_back( _folding_function_up.at(k) - _folding_function_down.at(k) );
	}
}

void SignalFilter::ApplyBandStopFilter()
 {
	_folding_function.clear();
	// TODO : some algorithm
	_folding_function.reserve( _folding_function_down.size() );
	_folding_function.push_back( 1.0 + _folding_function_down[0] - _folding_function_up[0] );
	// TODO : 
	for(int k = 0 ; k < _folding_function_up.size() ; k++)
	{
		_folding_function.push_back( _folding_function_down.at(k) - _folding_function_up.at(k) );
	}
}

void SignalFilter::FilterSerie(  PeriodicFunction* master_func
							   , PeriodicFunction* weight_func
							   , PeriodicFunction* tmp_func
							   , int weight_count
							   , const filter_t& filter)
{
	_weight_count = weight_count;
	switch (filter._type)
	{
	case LowFriquencyFilter:
		{
			WeightFunctionCount(_folding_function_up, filter._high_friq);
			// --- TMP --- 
			ValuesSet v;
			vector<double>::const_iterator it = _folding_function_up.begin();
			int i = -1 *  static_cast<int>( _folding_function_up.size()/2 );
			while ( it != _folding_function_up.end() )
			{
				v.push_back( make_pair( i++, (*it) ) );
				++it;
			}
			tmp_func->ResetWithSet(v);
			// --- /TMP --- 

			ApplyLowFrequencyFilter();
		}
		break;

	case HighFriquencyFilter:
		{
			WeightFunctionCount(_folding_function_down, filter._low_friq);

			// --- TMP --- 
			ValuesSet v;
			vector<double>::const_iterator it = _folding_function_down.begin();
			int i = -1 *  static_cast<int>( _folding_function_down.size()/2 );
			while ( it != _folding_function_down.end() )
			{
				v.push_back( make_pair( i++, (*it) ) );
				++it;
			}
			tmp_func->ResetWithSet(v);
			// --- /TMP --- 

			ApplyHighFrequencyFilter();

		}
		break;

	case BandFriquencyFilter:
		WeightFunctionCount(_folding_function_down, filter._low_friq);
		WeightFunctionCount(_folding_function_up, filter._high_friq);
		ApplyBandFilter();
		break;

	case BandStopFriquencyFilter:
		WeightFunctionCount(_folding_function_down, filter._low_friq);
		WeightFunctionCount(_folding_function_up, filter._high_friq);
		ApplyBandStopFilter();
		break;

	default:
		return;
	}

	WeightFunctionMirror(_folding_function);
	ValuesSet v;
	vector<double>::const_iterator it = _folding_function.begin();
	int i = -1 *  static_cast<int>( _folding_function.size()/2 );
	while ( it != _folding_function.end() )
	{
		v.push_back( make_pair( i++, (*it) ) );
		++it;
	}
	weight_func->ResetWithSet(v);
	
	UINT addr = -1;
	_beginthreadex(NULL, NULL, SignalFilter::Folding, master_func, NULL, &addr);
}

void SignalFilter::FilterJumps(PeriodicFunction* master_func, double dev_val)
{
	ValuesSet::iterator it = master_func->GetValuesSet().begin();
	while( it!= master_func->GetValuesSet().end() )
	{
		if( abs( (*it).second ) > abs( 2*dev_val ) )
		{
			(*it).second = ( (*(++it)).second + (*(--it)).second ) / 2; 
		}
		++it;
	}
}

void SignalFilter::FilterShifts(PeriodicFunction* master_func, double avg_val)
{
	if( abs(avg_val) > 10. )
	{
		ValuesSet::iterator it = master_func->GetValuesSet().begin();
		while( it!= master_func->GetValuesSet().end() )
		{
			(*it).second -= avg_val; 
			++it;
		}
	}
}
