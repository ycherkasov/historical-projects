// SettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SignalProcessing.h"
#include "SettingsDialog.h"
#include "SignalProcessingDlg.h"
#include "SerieSettings.h"


// SettingsDialog dialog

IMPLEMENT_DYNAMIC(SettingsDialog, CDialog)

SettingsDialog::SettingsDialog(CSignalProcessingDlg* master, CWnd* pParent /*=NULL*/)
	: CDialog(SettingsDialog::IDD, pParent)
	, _align_check_val(TRUE)
	, _correlate_check_val(FALSE)
	, _master_dlg(master)
	, _discrete_friq_value(0)
{

}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ALIGN_CHECK, _align_check_btn);
	DDX_Control(pDX, IDC_CORRELATE_CHECK, _correlate_check_btn);

	DDX_Check(pDX, IDC_ALIGN_CHECK, _align_check_val);
	DDX_Check(pDX, IDC_CORRELATE_CHECK, _correlate_check_val);
	DDX_Control(pDX, IDC_DISCRETE_FRIQ_SLIDER, _discrete_friq_slider);
	DDX_Slider(pDX, IDC_DISCRETE_FRIQ_SLIDER, _discrete_friq_value);
}

BOOL SettingsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	_master_dlg->SetSignalAlign(_align_check_val);
	_master_dlg->SetAutocorrelation(_correlate_check_val);
	_discrete_friq_slider.SetRange(1,3);
	return TRUE;
}


BEGIN_MESSAGE_MAP(SettingsDialog, CDialog)
	ON_BN_CLICKED(IDC_ALIGN_CHECK, &SettingsDialog::OnBnClickedAlignCheck)
	ON_BN_CLICKED(IDC_CORRELATE_CHECK, &SettingsDialog::OnBnClickedCorrelateCheck)
	ON_MESSAGE(WM_HSCROLL, OnMovement )
END_MESSAGE_MAP()


// SettingsDialog message handlers

void SettingsDialog::OnBnClickedAlignCheck()
{
	// —читать значени€ флажков
	UpdateData();
	_master_dlg->SetSignalAlign(_align_check_val);
}

void SettingsDialog::OnBnClickedCorrelateCheck()
{
	// —читать значени€ флажков
	UpdateData();
	_master_dlg->SetAutocorrelation(_correlate_check_val);
}

LRESULT SettingsDialog::OnMovement(WPARAM w, LPARAM l)
{
	if(w == TB_ENDTRACK)
	{
		// TODO : очистить все контейнеры и фильтры, сбросить настройки
		UpdateData();
		switch(_discrete_friq_value)
		{
		case 1: SerieSettings::SetSerieSize(1000); break;
		case 2: SerieSettings::SetSerieSize(5000); break;
		case 3: SerieSettings::SetSerieSize(11050); break;
		default: return 0l;
		}
		_master_dlg->ClearSignal();
	}
	return 0l;
}