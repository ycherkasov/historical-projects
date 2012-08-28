#pragma once
#include <vector>
#include <map>

class PeriodicFunction;

typedef std::map<int, int> pulse_points_t;

class PulseAttenuation
{
	static std::vector<double> _attenuation_func;
	static std::vector<double> _custom_attenuation_func;
	static UINT WINAPI Folding(void* pthis);
public:
	static void GeterateAttenuationSerie(PeriodicFunction* master_func, const pulse_points_t& att_settings);
};
