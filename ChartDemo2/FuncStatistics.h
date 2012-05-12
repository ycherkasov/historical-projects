#pragma once
#include <vector>
using namespace std;

class CPeriodicFunction;
class CChartDemo2Dlg;
class CFuncStatistics
{
	typedef std::vector<CPeriodicFunction*> FunctionsSet;
	typedef std::vector<std::pair<double,double> > ValuesSet;
	CChartDemo2Dlg* m_master;
	ValuesSet m_commonValues;
	ValuesSet m_valuesForCorrelation;
	ValuesSet m_FurieValues;
	FunctionsSet m_commonFunc;
	double m_average;
	double m_average_for_correlation;
	double m_sq_average;
	double m_dispersion;
	double m_deviation;
	bool m_recountCorrelationFunctions;
	bool m_useWhiteNoise;
	std::vector<double> m_accumulate;			// for average
	std::vector<double> m_double_accumulate;	// for self-correlation
	std::vector<double> m_mutual_accumulate;	// for self-correlation
	std::vector<double> m_white_noise;
protected:
	float FRand(float ran);
public:
	
	CFuncStatistics(CChartDemo2Dlg* master);
	~CFuncStatistics(void);
	static UINT WINAPI Recount(void* pthis);
	void AddSerie(CPeriodicFunction* func);
	void SetSerie();
	void DeleteSerie(CPeriodicFunction* func);
	double Average(){return m_average;}
	double AverageSquare(){return m_sq_average;}
	double Dispersion(){return m_dispersion;}
	double Deviation(){return m_deviation;}
	void WhiteNoise();
	void SaveCorrelation();

  // Fast Furie transformation
	void FurieTransform();
	void FurieTransformReverse();
	void FFT(const vector<int>& dIn, vector<double>& dOut, int nn, int beginData);
	void SetWhiteNoise(bool wn){ m_useWhiteNoise = wn; }
	void SetRecountCorrelation(BOOL recount){ m_recountCorrelationFunctions=(recount)?true:false; }
	void GetPeriodicFunction(int index, double& x, double& y);
	void GetFurieTransform(int index, double& x, double& y);
	double GetSelfCorreleted(int i){return m_double_accumulate.at(i);}
	double GetMutualCorreleted(int i){
		return m_mutual_accumulate.at(i);
	}
};
