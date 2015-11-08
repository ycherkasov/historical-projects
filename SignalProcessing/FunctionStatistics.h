#pragma once
#include <vector>
#include "HarmonicsComposition.h"
#include "PeriodicFunction.h"

class CSignalProcessingDlg;
class FunctionStatistics
{
public:
	FunctionStatistics(CDialog* dlg);
	~FunctionStatistics(void);
private:
	PeriodicFunction*		_master_function;
	CDialog*	_master_dlg;

	bool _recountCorrelationFunctions;

	double _average;
	double _sq_average;
	double _dispersion;
	double _deviation;

	std::vector<double> _accumulate;		// for average
	std::vector<double> _double_accumulate;	// for self-correlation
	std::vector<double> _mutual_accumulate;	// for mutual correlation

	PeriodicFunction	_values_for_correlation;
	double				_average_for_correlation;

	ValuesSet			_ft_values;

public:
	void SetMasterFunction(PeriodicFunction* pf){_master_function = pf;}
	static UINT WINAPI RecountStatistics(void* pthis);
	static UINT WINAPI FTThtead(void* pthis);
	static UINT WINAPI FFTThtead(void* pthis);
	void FillSelfCorrelation(ValuesSet& vs);
	void FillMutualCorrelation(ValuesSet& vs);
	void FurieTransform(/*ValuesSet& vs*/);
	void SetCorrelationRecount(bool is_recount);
	void SaveCorrelation();
	void ClearCorrelation();
	ValuesSet& GetFTValues(){return _ft_values;}

	double Average() const { return _average; } 
	double AverageSq() const { return _sq_average; } 
	double Dispersion()const { return _dispersion; } 
	double Deviation() const { return _deviation; } 
};
