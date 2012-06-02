#define _USE_MATH_DEFINES
#include "stdafx.h"
#include "ChartDemo2Dlg.h"
#include "FuncStatistics.h"
#include <cmath>
#include <process.h>


CFuncStatistics::CFuncStatistics(CChartDemo2Dlg* master) :
	  m_master(master)
	, m_average(0)
	, m_dispersion(0)
	, m_deviation(0)
	, m_useWhiteNoise(false)
{
	m_recountCorrelationFunctions = m_master->IsRecountAutocorr()?true:false;
}


CFuncStatistics::~CFuncStatistics()
{
}


UINT CFuncStatistics::Recount(void* pthis)
{
	CFuncStatistics* _this = static_cast<CFuncStatistics*>(pthis);
	double x = 0., y = 0.;
	double acum_average = 0;
	HWND hwnd = _this->m_master->GetSafeHwnd();
	::SendMessage(hwnd, WM_START_RECOUNT, 0, 0);

	// Count function values, average and square average
	_this->m_commonValues.clear();
	_this->m_double_accumulate.clear();

	for(int i = 0 ; i < SERIE_SIZE ; i++)
	{
		double acum_y = 0.;
		FunctionsSet::iterator it = _this->m_commonFunc.begin();
		while( it != _this->m_commonFunc.end() )
		{
			(*it)->GetPair(i,x,y);
			++it;
			acum_y += y;
		}
		
		if(_this->m_useWhiteNoise)
			acum_y += _this->m_white_noise.at(i);

		_this->m_commonValues.push_back( std::make_pair(x, acum_y) );
		acum_average += acum_y;
	}
	_this->m_average = acum_average/SERIE_SIZE;
	_this->m_sq_average = pow(_this->m_average,2);
	::SendMessage(hwnd, WM_GRAPHIC_COMPLETE, 0, 0);

	// Count dispersion and deviation
	ValuesSet::iterator vit = _this->m_commonValues.begin();
	double acum_dispersion = 0.;
	int progress_count = 0;

	while( vit != _this->m_commonValues.end())
	{
		// (Xk - Xavg) values
		double tmp1 = pow(((*vit).second - _this->m_average),2);
		_this->m_accumulate.push_back(tmp1);
		acum_dispersion += tmp1;

		// Count self-correlated and mutual-correlated functions
		double acum_autocorr = 0.;
		double acum_mutualcorr = 0.;
		if(_this->m_recountCorrelationFunctions)
		{
			for(int k = 0; k<SERIE_SIZE; k++)
			{
				int shift = k + progress_count;
				
				if( shift < SERIE_SIZE)
				{
					double tmp = (_this->m_commonValues.at(k).second - _this->m_average);
					acum_autocorr += 
						(tmp * (_this->m_commonValues.at(shift).second - _this->m_average) );
					if(!_this->m_valuesForCorrelation.empty())
					{
						acum_mutualcorr += 
							(tmp * (_this->m_valuesForCorrelation.at(shift).second - 
							_this->m_average_for_correlation) );
					}
				}
				else 
					continue;
			}
			acum_autocorr = acum_autocorr/SERIE_SIZE;
			acum_mutualcorr = acum_mutualcorr/SERIE_SIZE;

			_this->m_double_accumulate.push_back(acum_autocorr/100);
			_this->m_mutual_accumulate.push_back(acum_mutualcorr/100);
		}

		++vit;
		// Set progress-bar count
		::SendMessage(hwnd, WM_HOP_RECOUNT, (++progress_count), 0);
	}
	_this->m_dispersion = acum_dispersion/(double)SERIE_SIZE;
	_this->m_deviation = sqrt(_this->m_dispersion);

	// Hide progress and show graphics
	if(_this->m_valuesForCorrelation.empty())
		::SendMessage(hwnd, WM_END_RECOUNT, FALSE, 0);
	else
		::SendMessage(hwnd, WM_END_RECOUNT, TRUE, 0);


	return 0;
}


void CFuncStatistics::AddSerie(CPeriodicFunction* func)
{
	m_commonFunc.push_back(func);
	
	UINT addr = -1;
	_beginthreadex(NULL, NULL, CFuncStatistics::Recount, this,NULL,&addr);
}


void CFuncStatistics::SetSerie()
{
	UINT addr = -1;
	_beginthreadex(NULL, NULL, CFuncStatistics::Recount, this,NULL,&addr);
}

void CFuncStatistics::DeleteSerie(CPeriodicFunction* func)
{
	FunctionsSet::iterator it = m_commonFunc.begin();
	while(it != m_commonFunc.end())
	{
		if( (*it) == func )
		{
			m_commonFunc.erase(it);
			break;
		}
		++it;
	}
	UINT addr = -1;
	_beginthreadex(NULL, NULL, CFuncStatistics::Recount, this,NULL,&addr);
}

void CFuncStatistics::WhiteNoise()
{
	srand( static_cast<unsigned>(time(0)) );
	m_white_noise.clear();
	double RANGE_MIN = -10.;
	double RANGE_MAX = 10.;
	double rand_one = rand();
	for( int i = 0 ; i < SERIE_SIZE ; i++ )
	{
		rand_one = FRand(rand_one);
		double rand100 = rand_one * RANGE_MAX + RANGE_MIN;
		m_white_noise.push_back(rand100);
	}
	UINT addr = -1;
	m_useWhiteNoise = true;
	_beginthreadex(NULL, NULL, CFuncStatistics::Recount, this,NULL,&addr);

}

float CFuncStatistics::FRand(float ran)
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


void CFuncStatistics::GetPeriodicFunction(int index, double& x, double& y)
{
	x = m_commonValues.at(index).first;
	y = m_commonValues.at(index).second;
}

void CFuncStatistics::GetFurieTransform(int index, double& x, double& y)
{
	x = m_FurieValues.at(index).first;
	y = m_FurieValues.at(index).second;
}

void CFuncStatistics::SaveCorrelation()
{
	m_mutual_accumulate.clear();
	m_valuesForCorrelation.clear();
	m_valuesForCorrelation = m_commonValues;
	m_average_for_correlation = m_average;
}


void CFuncStatistics::FurieTransform()
{
	m_FurieValues.clear();
	size_t length = m_commonValues.size();
	for(unsigned i = 0 ; i < length/2 ; i++ )
	{
		int val = 0.;
		int val1 = 0.;
		int val2 = 0.;
		for( unsigned j = 0 ; j < length ; j++ )
		{
			val1 += (m_commonValues[j].second)*(cos( (2*M_PI*i*j)/length ) );
			val2 += (m_commonValues[j].second)*(sin( (2*M_PI*i*j)/length ) );
		}
		val = sqrt((double)val1*val1 + (double)val2*val2);
		val = val/10;
		m_FurieValues.push_back( std::make_pair(i, val) );
	}
}

void CFuncStatistics::FurieTransformReverse()
{
	m_commonValues.clear();
	size_t length = m_FurieValues.size();
	for(double i = 0.000 ; i <= 1.0 ; i+=0.001 )
	{
		double val = 0.;
		for( unsigned j = 0 ; j < length ; j++ )
		{
			val += (m_FurieValues[j].second)*( cos( (2*M_PI*i*j)/length ) );
		}
		val = val/length*2;
		m_commonValues.push_back( std::make_pair(i, val) );
	}

}

void CFuncStatistics::FFT(const vector<int>& dIn, vector<double>& dOut, int nn, int beginData)
{
  int i  = 0, j = 0, n = 0, m = 0, 
    mmax = 0, istep = 0;
  double tempr = 0., tempi = 0., wtemp = 0., theta = 0., 
    wpr = 0., wpi = 0., wr = 0., wi = 0.;

  int isign = -1;
  vector<double> data(nn*2 + 1);

  j = 0;
  for (i = beginData; i < beginData + nn; i++)
  {
    if (i < dIn.size())
    {
      data[j*2]   = 0;
      data[j*2+1] = dIn[i];
    }
    else
    {
      data[j*2]   = 0;
      data[j*2+1] = 0;
    }
    j++;
  }

  n = nn << 1;
  j = 1;
  i = 1;
  
  while (i < n)
  {
    if (j > i)
    {
      tempr = data[i];   data[i]   = data[j];   data[j]   = tempr;
      tempr = data[i+1]; data[i+1] = data[j+1]; data[j+1] = tempr;
    }
    m = n >> 1;
    while ((m >= 2) && (j > m))
    {
      j = j - m;
      m = m >> 1;
    }
    j = j + m;
    i = i + 2;
  }
  
  mmax = 2;
  while (n > mmax)
  {
    istep = 2 * mmax;
    theta = 2.0*M_PI / (isign * mmax);
    wtemp = sin(0.5 * theta);
    wpr   = -2.0 * wtemp * wtemp;
    wpi   = sin(theta);
    wr    = 1.0;
    wi    = 0.0;
    m    = 1;
    
    while (m < mmax)
    {
      i = m;
      while (i < n)
      {
        j         = i + mmax;
        tempr     = wr * data[j] - wi * data[j+1];
        tempi     = wr * data[j+1] + wi * data[j];
        data[j]   = data[i] - tempr;
        data[j+1] = data[i+1] - tempi;
        data[i]   = data[i] + tempr;
        data[i+1] = data[i+1] + tempi;
        i         = i + istep;
      }
      wtemp = wr;
      wr    = wtemp * wpr - wi * wpi + wr;
      wi    = wi * wpr + wtemp * wpi + wi;
      m     = m + 2;
    }
    mmax = istep;
  }

  for (i = 0; i < (nn / 2); i++)
  {
    dOut.push_back(sqrt( data[i*2] * data[i*2] + data[i*2+1] * data[i*2+1] ));
  }
}