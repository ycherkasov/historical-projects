#include "StdAfx.h"
#include "FunctionStatistics.h"
#include "SerieSettings.h"
#include "fft_src/Complex.h"
#include "fft_src/general.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <process.h>

FunctionStatistics::FunctionStatistics(CDialog* dlg)
	:	_master_function(0)
	,	_master_dlg(dlg)
	,	_average(0)
	,	_average_for_correlation(0)
	,	_sq_average(0)
	,	_dispersion(0)
	,	_deviation(0)
	,	_recountCorrelationFunctions(true)
{
}

FunctionStatistics::~FunctionStatistics(void)
{
}

UINT FunctionStatistics::RecountStatistics(void* pthis)
{
	FunctionStatistics* _this = static_cast<FunctionStatistics*>(pthis);
	if(!_this->_master_function)
		return 0l;

	HWND hwnd = _this->_master_dlg->GetSafeHwnd();
	int sz = _this->_master_function->Size();
	::SendMessage(hwnd, WM_START_RECOUNT, sz, 0);
	double acum_average = 0.;
	
	for( unsigned i = 0 ; i <  sz; i++ )
	{
		point_t p = _this->_master_function->GetValue(i);
		acum_average += p.second;
	}

	// --------------- Average --------------- 
	_this->_average = acum_average/_this->_master_function->Size();

	// --------------- Average squarte --------------- 
	_this->_sq_average = pow(_this->_average,2);

	// --------------- Corelation functions --------------- 
	double acum_dispersion = 0.;
	int progress_count = 0;

	if(_this->_recountCorrelationFunctions)
		_this->_double_accumulate.clear();

	for( unsigned i = 0 ; i < _this->_master_function->Size() ; i++)
	{
		// (Xk - Xavg) values
		double tmp1 = pow((  (_this->_master_function->GetValue(i).second ) - _this->_average),2);
		_this->_accumulate.push_back(tmp1);
		acum_dispersion += tmp1;

		// Count self-correlated and mutual-correlated functions
		double acum_autocorr = 0.;
		double acum_mutualcorr = 0.;
		if(_this->_recountCorrelationFunctions)
		{
			for(int k = 0; k<SerieSettings::SerieSize(); k++)
			{
				int shift = k + progress_count;

				if( shift < SerieSettings::SerieSize())
				{
					// Self correlation
					double tmp = (_this->_master_function->GetValue(k).second - _this->_average);
					acum_autocorr += 
						(tmp * (_this->_master_function->GetValue(shift).second - _this->_average) );

					// Mutual correlation
					if( _this->_values_for_correlation.Size() )
					{
						acum_mutualcorr += 
							(tmp * (_this->_values_for_correlation.GetValue(shift).second - 
							_this->_average_for_correlation) );
					}
				}
				else 
					continue;
			}
			acum_autocorr = acum_autocorr/SerieSettings::SerieSize();
			acum_mutualcorr = acum_mutualcorr/SerieSettings::SerieSize();

			// self- and mutual correlation
			_this->_double_accumulate.push_back(acum_autocorr/10);
			_this->_mutual_accumulate.push_back(acum_mutualcorr/10);
		}


		// Set progress-bar count
		::SendMessage(hwnd, WM_HOP_RECOUNT, (++progress_count), 0);
	}

	// --------------- Dispersion & deviation --------------- 
	_this->_dispersion = acum_dispersion/(double)SerieSettings::SerieSize();
	_this->_deviation = sqrt(_this->_dispersion);

	// Hide progress and show graphics
	if( _this->_values_for_correlation.Size() == 0 )
		::SendMessage(hwnd, WM_END_RECOUNT, FALSE, 0);
	else
		::SendMessage(hwnd, WM_END_RECOUNT, TRUE, 0);

	return 0;
}

UINT FunctionStatistics::FTThtead(void* pthis)
{
	FunctionStatistics* _this = static_cast<FunctionStatistics*>(pthis);
	
	_this->_ft_values.clear();
	
	HWND hwnd = _this->_master_dlg->GetSafeHwnd();
	size_t length = _this->_master_function->Size();
	::SendMessage(hwnd, WM_START_RECOUNT, length/2, 0);
	
	for(unsigned i = 0 ; i < length/2 ; i++ )
	{
		int val = 0.;
		int val1 = 0.;
		int val2 = 0.;
		for( unsigned j = 0 ; j < length ; j++ )
		{
			val1 += (_this->_master_function->GetValue(j).second)*(cos( (2*M_PI*i*j)/length ) );
			val2 += (_this->_master_function->GetValue(j).second)*(sin( (2*M_PI*i*j)/length ) );
		}
		val = sqrt((double)val1*val1 + (double)val2*val2);
		val = val/10;
		_this->_ft_values.push_back( std::make_pair(i, val) );
		::SendMessage(hwnd, WM_HOP_RECOUNT, i, 0);
	}
	::SendMessage(hwnd, WM_END_FT, FALSE, 0);
	return 0;
}

UINT FunctionStatistics::FFTThtead(void* pthis)
{
	FunctionStatistics* _this = static_cast<FunctionStatistics*>(pthis);

	_this->_ft_values.clear();

	HWND hwnd = _this->_master_dlg->GetSafeHwnd();
	size_t length = _this->_master_function->Size();
	::SendMessage(hwnd, WM_START_RECOUNT, length/2, 0);


	_this->_ft_values.reserve( length/2 );
	
	vector<double> r;
	for(unsigned i = 0 ; i < length ; i++ )
	{
		r.push_back( _this->_master_function->GetValue(i).second );
	}

	RealFFT(&r[0], length);
	for(unsigned i = 0 ; i < length/2 ; i++ )
	{
		::SendMessage(hwnd, WM_HOP_RECOUNT, i, 0);
		double val = r[i];
		_this->_ft_values.push_back( std::make_pair(i, val) );
	}

	::SendMessage(hwnd, WM_END_FT, FALSE, 0);
	return 0;
}

void FunctionStatistics::FillSelfCorrelation(ValuesSet& vs)
{
	vs.clear();

	std::vector<double>::const_iterator it = _double_accumulate.begin();
	for (double t = 0. ; (t < (SerieSettings::SerieSize()*SerieSettings::StepVal())) && (it != _double_accumulate.end()) ; t += SerieSettings::StepVal())
	{
		vs.push_back( std::make_pair(t, (*it) ) );
		++it;
	}
}


void FunctionStatistics::FillMutualCorrelation(ValuesSet& vs)
{
	vs.clear();
	vs.swap( _values_for_correlation.GetValuesSet() );
}


void FunctionStatistics::FurieTransform()
{
	UINT addr = -1;
	_beginthreadex(NULL, NULL, FunctionStatistics::FTThtead, this ,NULL, &addr);

}

void FunctionStatistics::SetCorrelationRecount(bool is_recount)
{
	_recountCorrelationFunctions = is_recount;
}

void FunctionStatistics::SaveCorrelation()
{
	_mutual_accumulate.clear();
	_values_for_correlation.ResetWithSet( _master_function->GetValuesSet() );
	_average_for_correlation = _average;
}

void FunctionStatistics::ClearCorrelation()
{
	_mutual_accumulate.clear();
	_values_for_correlation.Clear();
}

