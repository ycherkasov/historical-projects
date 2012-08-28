// FiltersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SignalProcessing.h"
#include "FiltersDialog.h"
#include "SignalProcessingDlg.h"

#include <cmath>

// FiltersDialog dialog

IMPLEMENT_DYNAMIC(FiltersDialog, CDialog)

FiltersDialog::FiltersDialog(CSignalProcessingDlg* master, CWnd* pParent /*=NULL*/)
	: CDialog(FiltersDialog::IDD, pParent)
	, _filter_type(0)
	, _high_bound_slider_value(0)
	, _low_bound_slider_value(0)
	, _master_dlg(master)
	, _is_show_weight(FALSE)
	, _weight_slider_value(0)
{

}

FiltersDialog::~FiltersDialog()
{
}

void FiltersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_LOW_FR_RADIO, _filter_type);
	DDX_Control(pDX, IDC_HIGH_FRIQ_SLIDER, _high_bound_slider);
	DDX_Control(pDX, IDC_LOW_FRIQ_SLIDER, _low_bound_slider);
	DDX_Control(pDX, IDC_HIGH_BOUND_STATIC, _high_bound_static);
	DDX_Control(pDX, IDC_LOW_BOUND_STATIC, _low_bound_static);
	DDX_Slider(pDX, IDC_HIGH_FRIQ_SLIDER, _high_bound_slider_value);
	DDX_Slider(pDX, IDC_LOW_FRIQ_SLIDER, _low_bound_slider_value);
	DDX_Control(pDX, IDC_SHOW_WEIGHT_BTN, _show_weight_btn);
	DDX_Control(pDX, IDC_WEIGHT_SLIDER, _weight_slider);
	DDX_Control(pDX, IDC_WEIGHT_STATIC, _weight_static);
	DDX_Slider(pDX, IDC_WEIGHT_SLIDER, _weight_slider_value);
}

BOOL FiltersDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	_weight_slider.SetRange(1, 5);
	_weight_slider.SetPos(3);
	CheckFilterType();
	SetStaticData();
	return TRUE;
}


BEGIN_MESSAGE_MAP(FiltersDialog, CDialog)
	ON_BN_CLICKED(IDC_CLEAN_JMP_BTN, &FiltersDialog::OnBnClickedCleanJmpBtn)
	ON_BN_CLICKED(IDC_CLEAN_BIAS_BTN, &FiltersDialog::OnBnClickedCleanBiasBtn)
	ON_BN_CLICKED(IDC_LOW_FR_RADIO, &FiltersDialog::OnBnClickedLowFrRadio)
	ON_BN_CLICKED(IDC_HIGH_FR_RADIO, &FiltersDialog::OnBnClickedHighFrRadio)
	ON_BN_CLICKED(IDC_BAND_FR_RADIO, &FiltersDialog::OnBnClickedBandFrRadio)
	ON_BN_CLICKED(IDC_BAND_ELIM_FR_RADIO, &FiltersDialog::OnBnClickedBandElimFrRadio)
	ON_BN_CLICKED(IDC_APPLY_FILTER_BUTTON, &FiltersDialog::OnBnClickedApplyFilterButton)

	ON_MESSAGE(WM_HSCROLL, OnMovement )
	ON_BN_CLICKED(IDC_SHOW_WEIGHT_BTN, &FiltersDialog::OnBnClickedShowWeightBtn)
END_MESSAGE_MAP()


void FiltersDialog::InitSlider()
{
	_high_bound_slider.SetRange(0, 100);
	_low_bound_slider.SetRange(0, 100);
}

// FiltersDialog message handlers

void FiltersDialog::OnBnClickedCleanJmpBtn()
{
	_master_dlg->CleanJumpNoise();
}

void FiltersDialog::OnBnClickedCleanBiasBtn()
{
	_master_dlg->CleanBiasNoise();
}

void FiltersDialog::OnBnClickedLowFrRadio()
{
	CheckFilterType();
}

void FiltersDialog::OnBnClickedHighFrRadio()
{
	CheckFilterType();
}

void FiltersDialog::OnBnClickedBandFrRadio()
{
	CheckFilterType();
}

void FiltersDialog::OnBnClickedBandElimFrRadio()
{
	CheckFilterType();
}

void FiltersDialog::OnBnClickedApplyFilterButton()
{
	if( (_high_bound_value < _low_bound_value) && (_filter_type > 1) )
	{
		::MessageBox(NULL, L"Нижняя граница частотного фильтра больше верхней, исправьте настройки",
			L"Ошибка настроек фильтра", MB_OK | MB_ICONWARNING);
	}

	filter_t filter(_filter_type, _high_bound_value, _low_bound_value);
	_master_dlg->GenerateFilteredSignal(filter);
}

void FiltersDialog::OnBnClickedShowWeightBtn()
{
	(_is_show_weight = !_is_show_weight)
		?(_show_weight_btn.SetWindowText(L"Скрыть"))
		:(_show_weight_btn.SetWindowText(L"Отобразить"));
	_master_dlg->ChangeSignalInformationChart(_is_show_weight);
}

LRESULT FiltersDialog::OnMovement(WPARAM w, LPARAM l)
{
	SetStaticData();
	if(w == TB_ENDTRACK)
	{
		// TODO : возможно, пересчитать фильтр
	}
	return 0l;
}


void FiltersDialog::CheckFilterType()
{
	UpdateData();
	_high_bound_slider.EnableWindow(FALSE);
	_low_bound_slider.EnableWindow(FALSE);

	switch (_filter_type)
	{
	case LowFriquencyFilter:
		_high_bound_slider.EnableWindow(TRUE);
		break;

	case HighFriquencyFilter:
		_low_bound_slider.EnableWindow(TRUE);
		break;

	case BandFriquencyFilter:
	case BandStopFriquencyFilter:
		_high_bound_slider.EnableWindow(TRUE);
		_low_bound_slider.EnableWindow(TRUE);
		break;

	default:
		return;
	}
}

void FiltersDialog::CountFriquencyValues()
{
	_high_bound_value = _high_bound_slider_value*5;
	_low_bound_value = _low_bound_slider_value*5;
	_weight_value = pow(2.,_weight_slider_value + 4);
}

void FiltersDialog::SetStaticData()
{
	UpdateData();
	wchar_t str[255] = {0};
	CountFriquencyValues();
	_high_bound_static.SetWindowText( _itow(_high_bound_value, str, 10) ) ;
	_low_bound_static.SetWindowText( _itow(_low_bound_value, str, 10) ) ;
	_weight_static.SetWindowText( _itow(_weight_value, str, 10) ) ;
}

int FiltersDialog::GetWeightFunctionCount()
{
	return _weight_value;
}

