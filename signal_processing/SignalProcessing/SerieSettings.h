#pragma once

class SerieSettings
{
	static int serie_size;
	static double step_val;
public:
	static int& SerieSize(){ 
		return serie_size; 
	}
	static double& StepVal(){ 
		return step_val; 
	}
	static void SetSerieSize(int new_size);
};
