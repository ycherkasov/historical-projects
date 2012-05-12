// FuncDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDemo2.h"
#include "FuncDlg.h"
#include "ChartDemo2Dlg.h"
#include "PeriodicFunction.h"


// CFuncDlg dialog

IMPLEMENT_DYNAMIC(CFuncDlg, CDialog)

CFuncDlg::CFuncDlg(int sel, 
		CPeriodicFunction* func, 
		CChartDemo2Dlg* master, 
		CFuncDlg_Mode mode, 
		CWnd* pParent /*=NULL*/)
	: CDialog(CFuncDlg::IDD, pParent)
	, m_mode(mode)
	, m_master(master)
	, m_relatedFunction(func)
	, m_selected_line(sel)
{
	if(m_relatedFunction)
	{
		m_a0_value = m_relatedFunction->m_a0;
		m_f0_value = m_relatedFunction->m_f0/100;
		m_dt_value = m_relatedFunction->m_dt*1000;
	}
	else
	{
		m_a0_value = 1;
		m_f0_value = 1;
		m_dt_value = 1;
	}
}

CFuncDlg::~CFuncDlg()
{
}

void CFuncDlg::DoDataExchange(CDataExchange* pDX)
{
  DDX_Control(pDX, IDC_SLIDER_F0, m_f0_slider);
  DDX_Control(pDX, IDC_SLIDER_DT, m_dt_slider);
  DDX_Control(pDX, IDC_SLIDER_A0, m_a0_slider);

  DDX_Control(pDX, IDC_STATIC_F0, m_f0_stvalue);
  DDX_Control(pDX, IDC_STATIC_DT, m_dt_stvalue);
  DDX_Control(pDX, IDC_STATIC_A0, m_a0_stvalue);

  DDX_Slider(pDX, IDC_SLIDER_F0, m_f0_value);
  DDX_Slider(pDX, IDC_SLIDER_DT, m_dt_value);
  DDX_Slider(pDX, IDC_SLIDER_A0, m_a0_value);

	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFuncDlg, CDialog)
	ON_MESSAGE(WM_HSCROLL, OnMovement )
END_MESSAGE_MAP()


// CFuncDlg message handlers

BOOL CFuncDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	InitSliders();
	UpdateData(FALSE);
	CountFunctionValues();
	SetStaticData();

	return TRUE;
}

// ----------------------------------
void CFuncDlg::InitSliders()
{
  m_a0_slider.SetRange(1, 50);   //*1
  m_f0_slider.SetRange(1, 100); //*5
  m_dt_slider.SetRange(1, 100); //*0.001
}

void CFuncDlg::CountFunctionValues()
{
  m_a0_func_value = m_a0_value;
  m_dt_func_value = static_cast<double>(m_dt_value) * 0.001;
  m_f0_func_value = m_f0_value*5;
}

void CFuncDlg::SetStaticData()
{
  wchar_t str[255] = {0};
  m_a0_stvalue.SetWindowText( _itow(m_a0_func_value, str, 10) ) ;
  m_f0_stvalue.SetWindowText( _itow(m_f0_func_value, str, 10) ) ;
  swprintf( str, 255, L"%.3f", m_dt_func_value );
  m_dt_stvalue.SetWindowText( str ) ;
}

LRESULT CFuncDlg::OnMovement(WPARAM w, LPARAM l)
{
	UpdateData();
	CountFunctionValues();
	SetStaticData();
	return 0l;
}


void CFuncDlg::OnOK()
{
	if(m_mode == mode_add)
	{
		m_master->AddLine(m_a0_func_value, m_f0_func_value, m_dt_func_value);
	}
	else if(m_mode == mode_edit)
	{
		m_master->SetLine(m_selected_line, m_a0_func_value, m_f0_func_value, m_dt_func_value);
	}
	
	CDialog::OnOK();
}