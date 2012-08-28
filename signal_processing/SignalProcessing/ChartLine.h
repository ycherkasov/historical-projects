#pragma once
#include "PeriodicFunction.h"

class CChartLineSerie;
class CChartCtrl;

/** @brief	class ChartLine encapsulates properties set
			for ChartCtrl function (color, align, values)
*/
class ChartLine
{
	/** @brief Link to parent chart control */
	CChartCtrl*			_chart_ctrl;

	/** @brief Link to chart line present current function */
	CChartLineSerie*	_line;

	/** @brief Line color */
	COLORREF			_line_color;

	/** @brief This object contains function values */
	PeriodicFunction	_function_values;

	/** @brief Align function serie by max-min values property */
	bool				_align_by_val;

public:
	ChartLine(CChartCtrl* ctrl, COLORREF c);

	/** @brief Add point to function series */
	void AddPoint(double X, double Y);

	/** @brief Redraw function series and align if required */
	void UpdateSerie();

	/** @brief Helper function */
	PeriodicFunction* GetFunctionSerie(){return &_function_values;}

	/** @brief Set align at parent Chart control */
	void SetAlign(bool is_align){ _align_by_val = is_align; }

};
