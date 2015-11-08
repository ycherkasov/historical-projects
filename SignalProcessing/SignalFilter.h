#pragma once
#include <vector>

enum FilterType
{
	LowFriquencyFilter,
	HighFriquencyFilter,
	BandFriquencyFilter,
	BandStopFriquencyFilter,
	FiltersCount
};

class PeriodicFunction;

struct filter_t
{
	int _type;
	unsigned _high_friq;
	unsigned _low_friq;
	filter_t(int t, unsigned high, unsigned low) :
	_type(t), _high_friq(high), _low_friq(low){}
};

class SignalFilter
{
	static double	_tmp_dev;
	static int		_weight_count;
	static std::vector<double> _folding_function;
	static std::vector<double> _folding_function_down;
	static std::vector<double> _folding_function_up;
public:
	static UINT WINAPI Folding(void* pthis);
	static void WeightFunctionCount(std::vector<double>& folding_output, unsigned frequency);
	static void WeightFunctionMirror(std::vector<double>& folding_output);

	static void ApplyLowFrequencyFilter();
	static void ApplyHighFrequencyFilter();
	static void ApplyBandFilter();
	static void ApplyBandStopFilter();

	static void FilterSerie( PeriodicFunction* master_func
		, PeriodicFunction* weight_func
		, PeriodicFunction* tmp_func
		, int weight_count
		, const filter_t& filter );
	static void FilterJumps( PeriodicFunction* master_func, double dev_val );
	static void FilterShifts( PeriodicFunction* master_func, double avg_val );
};
