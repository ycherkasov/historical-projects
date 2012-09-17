// StatisticDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SignalProcessing.h"
#include "StatisticDialog.h"


// StatisticDialog dialog

IMPLEMENT_DYNAMIC(StatisticDialog, CDialog)

StatisticDialog::StatisticDialog(CWnd* pParent /*=NULL*/)
	: CDialog(StatisticDialog::IDD, pParent)
{

}

StatisticDialog::~StatisticDialog()
{
}

void StatisticDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AVG2_STATIC, _average_static);
	DDX_Control(pDX, IDC_AVGS2_STATIC, _average_sq_static);
	DDX_Control(pDX, IDC_DISP2_STATIC, _dispersion_static);
	DDX_Control(pDX, IDC_DEV2_STATIC, _deviation_static);
}


BEGIN_MESSAGE_MAP(StatisticDialog, CDialog)
END_MESSAGE_MAP()


// StatisticDialog message handlers

void StatisticDialog::SetStaticData(double avg, double avgs, double disp, double dev)
{
	wchar_t str[255] = {0};

	swprintf( str, 255, L"%.4f", avg );
	_average_static.SetWindowText( str ) ;

	swprintf( str, 255, L"%.4f", avgs );
	_average_sq_static.SetWindowText( str ) ;

	swprintf( str, 255, L"%.4f", disp );
	_dispersion_static.SetWindowText( str ) ;

	swprintf( str, 255, L"%.4f", dev );
	_deviation_static.SetWindowText( str ) ;
}

