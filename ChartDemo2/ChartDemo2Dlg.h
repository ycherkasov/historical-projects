// ChartDemo2Dlg.h : header file
//

#pragma once
#include "ChartCtrl.h"
#include "ColourPicker.h"
#include "PeriodicFunction.h"
#include "FuncStatistics.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

static const int SERIE_SIZE = 1000;

enum SerieType
{
		harmonic_type
	,	furie_type
};

// CChartDemo2Dlg dialog
class CChartDemo2Dlg : public CDialog
{
	// Construction
	CListCtrl m_funcList;
	SerieType m_serieType;

	CChartCtrl m_ChartCtrl;
	CChartLineSerie* m_pLine;
	CChartLineSerie* m_pLineAutocorr;
	CChartLineSerie* m_pLineMutualcorr;
	CChartLineSerie* m_pLineFT;
	CFuncStatistics m_stat;

	int m_f0_func_value;
	double m_dt_func_value;
	int m_a0_func_value;

	CButton m_align_control;
	CButton m_autocorrButton;
	CButton m_addharmButton;
	CButton m_remharmButton;
	CButton m_editharmButton;
	CButton m_mutualcorrButton;
	CButton m_whitenoiseButton;
	CButton m_FTButton;
	CButton m_exitButton;

	CMenu m_mainMenu;

	BOOL m_align_val;
	BOOL m_recountCorrelate;

	CStatic m_average;
	CStatic m_sq_average;
	CStatic m_dispersion;
	CStatic m_deviation;

	CStatic m_progressCaption;
	CProgressCtrl m_recountProgress;

public:
	CChartDemo2Dlg(CWnd* pParent = NULL);	// standard constructor
	~CChartDemo2Dlg();

	// Dialog Data
	enum { IDD = IDD_CHARTDEMO2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void InitChart();

	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()

	// Control list methods

public:
	void UpdateSerie(bool autocorr, bool mutual_corr);
	void UpdateFurie();

	void AddLine(int a0, int f0, double dt);
	void SetLine(int nLine, int a0, int f0, double dt);
	void DeleteLine(int nLine);
	void SetStaticData();
	void ShowProgress(BOOL show);

	LRESULT OnStartRecount(WPARAM w, LPARAM l);
	LRESULT OnHopRecount(WPARAM w, LPARAM l);
	LRESULT OnGraphicComplete(WPARAM w, LPARAM l);
	LRESULT OnRecountComplete(WPARAM w, LPARAM l);

	afx_msg void OnBnClickedBtnAddfunc();
	afx_msg void OnBnClickedBtnRmfunc();
	afx_msg void OnBnClickedBtnEditfunc();

	BOOL IsRecountAutocorr(){ return m_recountCorrelate; }

	afx_msg void OnBnClickedMutualcorrButton();
	afx_msg void OnBnClickedWhitenoiseButton();

public:
	afx_msg void OnBnClickedFft();
	afx_msg void OnFileExit();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
};
