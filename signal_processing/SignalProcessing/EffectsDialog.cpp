// EffectsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SignalProcessing.h"
#include "EffectsDialog.h"
#include "NoiseGenerator.h"
#include "SignalProcessingDlg.h"

#include <string>

using namespace std;
// EffectsDialog dialog

IMPLEMENT_DYNAMIC(EffectsDialog, CDialog)
// ---------------  --------------- 
EffectsDialog::EffectsDialog(CSignalProcessingDlg* master, CWnd* pParent /*=NULL*/)
	: CDialog(EffectsDialog::IDD, pParent)
	, _master_dlg(master)
	, _point_slider_value(0)
{
}

// ---------------  --------------- 
EffectsDialog::~EffectsDialog()
{
}

// ---------------  --------------- 
void EffectsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINT_EDIT, m_point_edit);
	DDX_Control(pDX, IDC_ADD_POINT, m_add_point);
	DDX_Control(pDX, IDC_DELETE_POINT, m_del_point);
	DDX_Control(pDX, IDC_CLEAR_POINTS, m_clean_points);
	DDX_Control(pDX, IDC_POINT_LIST, m_points_list);
	DDX_Control(pDX, IDC_POINT_VAL_SLIDER, _point_slider);
	DDX_Control(pDX, IDC_POINT_VAL_STATIC, _point_value_static);
	DDX_Control(pDX, IDC_KNOCK_APPLY_BTN, m_apply_pulse_btn);

	DDX_Slider(pDX, IDC_POINT_VAL_SLIDER, _point_slider_value);
}

// ---------------  --------------- 
BOOL EffectsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	_point_value_static.SetWindowText(L"0");
	_point_slider.SetRange(0, 100);
	m_point_edit.SetLimitText(4);
	CheckItemsSelected();
	return TRUE;
}

// ---------------  --------------- 
BEGIN_MESSAGE_MAP(EffectsDialog, CDialog)
	ON_BN_CLICKED(IDC_ADD_POINT, &EffectsDialog::OnBnClickedAddPoint)
	ON_EN_CHANGE(IDC_POINT_EDIT, &EffectsDialog::OnEnChangePointEdit)
	ON_LBN_SELCHANGE(IDC_POINT_LIST, &EffectsDialog::OnLbnSelchangePointList)
	ON_BN_CLICKED(IDC_DELETE_POINT, &EffectsDialog::OnBnClickedDeletePoint)
	ON_BN_CLICKED(IDC_CLEAR_POINTS, &EffectsDialog::OnBnClickedClearPoints)
	ON_BN_CLICKED(IDC_WHITE_NOISE_BTN, &EffectsDialog::OnBnClickedWhiteNoiseBtn)
	ON_BN_CLICKED(IDC_JUMP_NOISE_BTN, &EffectsDialog::OnBnClickedJumpNoiseBtn)
	ON_BN_CLICKED(IDC_KNOCK_APPLY_BTN, &EffectsDialog::OnBnClickedKnockApplyBtn)
	ON_BN_CLICKED(IDC_MUTUAL_SAVE_BTN, &EffectsDialog::OnBnClickedMutualSaveBtn)
	ON_BN_CLICKED(IDC_MUTUAL_CLEAR_BTN, &EffectsDialog::OnBnClickedMutualClearBtn)
	ON_MESSAGE(WM_VSCROLL, OnMovement )
	ON_BN_CLICKED(IDC_SHIFT_NOISE_BTN, &EffectsDialog::OnBnClickedShiftNoiseBtn)
END_MESSAGE_MAP()


// ---------------  --------------- 
void EffectsDialog::OnBnClickedAddPoint()
{
	CString s;
	m_point_edit.GetWindowText(s);
	if(!s.IsEmpty())
	{
		m_points_list.AddString( s );
		_points_with_values[ _wtoi(s) ] = 0;
		m_point_edit.SetWindowText(L"");
	}
	CheckItemsSelected();
}

// ---------------  --------------- 
void EffectsDialog::OnBnClickedDeletePoint()
{
	int sel = m_points_list.GetCurSel();
	if(sel != LB_ERR)
	{
		wchar_t str[255] = {0};
		m_points_list.GetText( sel, str );
		int pt = _wtoi(str);
		_points_with_values.erase( _points_with_values.find(pt) ) ;
	}
	m_points_list.DeleteString(sel);

	CheckItemsSelected();
}

// ---------------  --------------- 
void EffectsDialog::OnBnClickedClearPoints()
{
	m_points_list.ResetContent();
	CheckItemsSelected();
}

// ---------------  --------------- 
void EffectsDialog::OnEnChangePointEdit()
{
	CString s;
	m_point_edit.GetWindowText(s);
	
	if(s.IsEmpty())
		m_add_point.EnableWindow(FALSE);
	else
		m_add_point.EnableWindow(TRUE);
}

// ---------------  --------------- 
void EffectsDialog::CheckItemsSelected()
{
	if(m_points_list.GetCurSel() == LB_ERR)
		SetSliderPosition(0);

	m_del_point.EnableWindow(!(m_points_list.GetCurSel() == LB_ERR));
	_point_slider.EnableWindow(!(m_points_list.GetCurSel() == LB_ERR));
	m_apply_pulse_btn.EnableWindow( m_points_list.GetCount() );
	m_clean_points.EnableWindow( m_points_list.GetCount() );
}

void EffectsDialog::SetSliderPosition(int pos)
{
	_point_slider.SetPos(pos);
	SetStaticData();
}

void EffectsDialog::SetStaticData()
{
	UpdateData();
	wchar_t str[255] = {0};
	_point_value_static.SetWindowText( _itow(_point_slider_value, str, 10) ) ;

}

// ---------------  --------------- 
void EffectsDialog::OnLbnSelchangePointList()
{
	CheckItemsSelected();
	SetSliderPosition( m_points_list.GetItemData( m_points_list.GetCurSel() ) );
}

void EffectsDialog::OnBnClickedWhiteNoiseBtn()
{
	std::vector<double> noise_vals;
	NoiseGenerator::GenerateWhiteNoise(noise_vals);
	_master_dlg->_main_signal.GetFunctionSerie()->AddHarmonic(noise_vals);
	_master_dlg->Recount();
}

void EffectsDialog::OnBnClickedJumpNoiseBtn()
{
	std::vector<double> noise_vals;
	NoiseGenerator::GenerateJumpNoise(noise_vals);
	_master_dlg->_main_signal.GetFunctionSerie()->AddHarmonic(noise_vals);
	_master_dlg->Recount();

}

void EffectsDialog::OnBnClickedShiftNoiseBtn()
{
	std::vector<double> shift_vals;
	NoiseGenerator::GenerateShiftNoise(shift_vals);
	_master_dlg->_main_signal.GetFunctionSerie()->AddHarmonic(shift_vals);
	_master_dlg->Recount();
}


void EffectsDialog::OnBnClickedKnockApplyBtn()
{
	// generate pulse signal here
	//_test_points[200] = 70;
	//_test_points[400] = 40;
	//_test_points[600] = 120;
	_master_dlg->GenerateAttenuationSignal(_points_with_values /*_test_points*/);
}

// ---------------  --------------- 
LRESULT EffectsDialog::OnMovement(WPARAM w, LPARAM l)
{
	SetStaticData();
	if(w == TB_ENDTRACK)
	{
		// установить связанное значение в списке
		wchar_t str[255] = {0};
		
		m_points_list.GetText( m_points_list.GetCurSel(), str );
		int pt = _wtoi(str);
		_points_with_values[pt] = _point_slider_value;
		
		m_points_list.SetItemData(m_points_list.GetCurSel(), _point_slider_value);
	}
	return 0l;
}

void EffectsDialog::OnBnClickedMutualSaveBtn()
{
	_master_dlg->SaveMutualCorrelation();
}

void EffectsDialog::OnBnClickedMutualClearBtn()
{
	_master_dlg->ClearMutualCorrelation();
}

