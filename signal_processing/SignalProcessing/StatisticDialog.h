#pragma once
#include "afxwin.h"


// StatisticDialog dialog

class StatisticDialog : public CDialog
{
	DECLARE_DYNAMIC(StatisticDialog)

public:
	StatisticDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~StatisticDialog();

// Dialog Data
	enum { IDD = IDD_STAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CStatic _average_static;
	CStatic _average_sq_static;
	CStatic _dispersion_static;
	CStatic _deviation_static;

public:
	void SetStaticData(double avg, double avgs, double disp, double dev);
};
