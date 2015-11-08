#include "StdAfx.h"
#include "ChartLine.h"

#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartCtrl.h"

// ------------------------------ 
ChartLine::ChartLine( CChartCtrl* ctrl, COLORREF c ) :
	  _line_color(c)
	, _chart_ctrl(ctrl)
	, _line(0)		
	, _align_by_val(false)
{
	// Set '_line' pointer to zero cause it can be 
	// uninitialized at moment of creation 'ChartLine' object
}

// ------------------------------ 
void ChartLine::AddPoint(double X, double Y)
{
	_line->AddPoint(X, Y);
}

// ---------------  --------------- 
void ChartLine::UpdateSerie()
{
	double _max = 0.;
	double _min = 0.;

	_line = (CChartLineSerie*)_chart_ctrl->AddSerie(CChartSerie::stLineSerie);
	_line->SetSeriesOrdering(CChartSerie::soNoOrdering);
	_line->SetColor(_line_color);

	// Left and right bound for align
	double x_max = 0.;
	double x_min = 0.;
	if(_function_values.Size())
		x_min = _function_values.GetValue(0).first;

	for ( unsigned t=0 ; t < _function_values.Size() ; t++ )
	{
		point_t p = _function_values.GetValue(t);
		_line->AddPoint(p.first, p.second);

		x_max = p.first;

		// Get upper and lower bound for align
		if(p.second > _max)
			_max = p.second;

		if(p.second < _min)
			_min = p.second;
	}


	// If align set and function series not [ y = 0 ]
	if(_align_by_val && (_min || _max) )
	{
		_chart_ctrl->GetBottomAxis()->SetMinMax(x_min,x_max);

		double y_delay = max( abs(_max), abs(_min) );
		double y_max = _max + abs(y_delay/3);
		double y_min = _min - abs(y_delay/3);
		_chart_ctrl->GetLeftAxis()->SetMinMax( y_min , y_max );
	}
}
