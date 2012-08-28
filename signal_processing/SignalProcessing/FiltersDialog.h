#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SignalFilter.h"


// FiltersDialog dialog

class CSignalProcessingDlg;


class FiltersDialog : public CDialog
{
	DECLARE_DYNAMIC(FiltersDialog)

public:
	FiltersDialog(CSignalProcessingDlg* master, CWnd* pParent = NULL);   // standard constructor
	virtual ~FiltersDialog();

// Dialog Data
	enum { IDD = IDD_FILTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

// ------------------------------ 
private:
	CSignalProcessingDlg* _master_dlg;

	// --- Filter --- 
	/** @brief Radio button param */
	int _filter_type;

	BOOL _is_show_weight;

	//  --- Frequency ---
	CSliderCtrl _high_bound_slider;
	CSliderCtrl _low_bound_slider;
	
	int _high_bound_slider_value;
	int _low_bound_slider_value;
	int _high_bound_value;
	int _low_bound_value;

	CStatic _high_bound_static;
	CStatic _low_bound_static;

	//  --- Weight function --- 
	CButton _show_weight_btn;
	CSliderCtrl _weight_slider;
	int _weight_slider_value;
	
	CStatic _weight_static;
	int _weight_value;


protected:
	void InitSlider();

	afx_msg void OnBnClickedCleanJmpBtn();
	afx_msg void OnBnClickedCleanBiasBtn();
	afx_msg void OnBnClickedLowFrRadio();
	afx_msg void OnBnClickedHighFrRadio();
	afx_msg void OnBnClickedBandFrRadio();
	afx_msg void OnBnClickedBandElimFrRadio();
	afx_msg void OnBnClickedApplyFilterButton();
	afx_msg void OnBnClickedShowWeightBtn();
	afx_msg LRESULT OnMovement(WPARAM w, LPARAM l);

	void CheckFilterType();
	void inline CountFriquencyValues();
	void SetStaticData();

public:
	int GetWeightFunctionCount();
	
	
};
