#pragma once

#include "HarmonicsComposition.h"
#include "ChartLine.h"


// HarmonicDialog dialog

class CSignalProcessingDlg;

class HarmonicDialog : public CDialog
{
	DECLARE_DYNAMIC(HarmonicDialog)

public:
	HarmonicDialog(CSignalProcessingDlg* master, CWnd* pParent = NULL);   // standard constructor
	virtual ~HarmonicDialog();

// Dialog Data
	enum { IDD = IDD_HARMONICS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void InitHarmonicsList();
	void InitSliders();


	DECLARE_MESSAGE_MAP()
private:
	CSignalProcessingDlg* _master;

	CListCtrl m_funcList;

	CSliderCtrl m_a0_slider;
	CSliderCtrl m_f0_slider;

	// Текущие значения слайдеров
	int m_a0_value;
	int m_f0_value;

	// Перечситанные значения функций
	int m_a0_func_value;
	int m_f0_func_value;

	CStatic m_a0_stvalue;
	CStatic m_f0_stvalue;

	FunctionsSet _harmonics;

protected:

	// List elements control
	void AddLine();
	void SetLine(int nLine);
	void DeleteLine(int index);
	int GetSelectedLine();

	// Attached data control
	ListHarmonicData* AddListItemData(int nItem, int a0, int f0);
	void DeleteListItemData(int nItem);
	void CleanListItemData();

	// Sliders control
	void SetSlidersPos(int a_pos, int f_pos);
	void EnableSliders(BOOL enable);

	// Recount harmonic function values
	// according to sliders values
	void CountFunctionValues();
	
	// Set static values
	void SetStaticData();

	// recount harmonic and correlation values
	void RecountHarmonics();
	void UpdateControls();
	
	afx_msg LRESULT OnMovement(WPARAM w, LPARAM l);
	afx_msg void OnBnClickedAddHarmonic();
	afx_msg void OnBnClickedDelHarmonic();
	afx_msg void OnBnClickedCleanHarmonic();
	afx_msg void OnLvnItemchangedFuncList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
public:
	void CleanHarmonics();
};
