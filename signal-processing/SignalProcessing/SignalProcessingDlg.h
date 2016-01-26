// SignalProcessingDlg.h : header file
//

#pragma once
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ColourPicker.h"

#include "ChartLine.h"
#include "FunctionStatistics.h"

#include "HarmonicDialog.h"
#include "EffectsDialog.h"
#include "StatisticDialog.h"
#include "SettingsDialog.h"
#include "FiltersDialog.h"

#include "afxwin.h"
#include "afxcmn.h"


// CSignalProcessingDlg dialog
class CSignalProcessingDlg : public CDialog
{
	
// Construction
public:
	CSignalProcessingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SIGNALPROCESSING_DIALOG };

	enum DialogSheets
	{
		HarmonicsDlgSheet,
		EffectsDlgSheet,
		FiltersDlgSheet,
		StatisticDlgSheet,
		SettingsDlgSheet,
		DlgSheetCount
	};
	CDialog* dlgs[DlgSheetCount];

	struct ChartColorScheme
	{
		COLORREF BackColor;
		COLORREF GridColor;
		COLORREF TextColor;
	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// -------- User-declared section -------- 
private:

	RECT	_settingsRect;
	RECT	_childDialogRect;
	RECT	_recountStaticRect;
	RECT	_recountProgressRect;
	RECT	_signalChartRect;
	RECT	_spectrumChartRect;

	CChartCtrl	_signalCtrl;
	CChartCtrl	_spectrumCtrl;
	CChartCtrl	_weightCtrl;

	CStatic _recount_static;
	CProgressCtrl _recount_prgress;

	CTabCtrl	_settingsTab;

	CMenu _mainMenu;

	FunctionStatistics	_statistic;

	HarmonicDialog		_harmonic_dialog;
	EffectsDialog		_effect_dialog;
	StatisticDialog		_statistic_dialog;
	SettingsDialog		_settings_dialog;
	FiltersDialog		_filters_dilaog;

	friend class HarmonicDialog;
	friend class EffectsDialog;
	
	ChartLine			_main_signal;
	ChartLine			_auto_correlation;
	ChartLine			_mutual_correlation;
	ChartLine			_signal_spectrum;
	ChartLine			_weight_function;

	ChartLine			_tmp_function;

	bool				_is_auto_correlate;
	bool				_is_mutual_correlate;
protected:
	afx_msg void OnTcnSelchangeSettingsTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileExit();
	afx_msg void OnHelpAbout();

public:

	void InitTabs();
	void InitTab(UINT index, LPWSTR caption);
	void InitChart(CChartCtrl& ctrl, LPCTSTR title, ChartColorScheme& cs);
	void InitCharts();
	void CreateChildDialogs();
	void Sizing();
	void WindowPlacement(CWnd* wnd, RECT& rect);
	void UpdateSerie();
	void UpdateSpectrum();
	void UpdateWeight();
	void SetSignalAlign(BOOL algn);
	void SetAutocorrelation(BOOL autocorr);
	void Recount();
	void GenerateAttenuationSignal(const pulse_points_t& att_settings);
	void GenerateFilteredSignal(const filter_t& filter);
	void CleanJumpNoise();
	void CleanBiasNoise();
	void RecountSignal(const ValuesSet& vals);
	void FinishRecount(bool also_correlate);
	void ChangeSignalInformationChart(BOOL is_weight);

	LRESULT OnStartRecount(WPARAM w, LPARAM l);
	LRESULT OnHopRecount(WPARAM w, LPARAM l);
	LRESULT OnRecountComplete(WPARAM w, LPARAM l);
	LRESULT OnFTComplete(WPARAM w, LPARAM l);
	
	void ShowProgress(BOOL show);
	void SaveMutualCorrelation();
	void ClearMutualCorrelation();

	int GetWeightFunctionCount();
	void ClearSignal();
};
