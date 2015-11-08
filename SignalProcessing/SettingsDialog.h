#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// SettingsDialog dialog
class CSignalProcessingDlg;

class SettingsDialog : public CDialog
{
	DECLARE_DYNAMIC(SettingsDialog)

public:
	SettingsDialog(CSignalProcessingDlg* master, CWnd* pParent = NULL);   // standard constructor
	virtual ~SettingsDialog();

// Dialog Data
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CSignalProcessingDlg*	_master_dlg;

	CButton _align_check_btn;
	CButton _correlate_check_btn;

	BOOL _align_check_val;
	BOOL _correlate_check_val;

	CSliderCtrl _discrete_friq_slider;
	int _discrete_friq_value;

protected:
	afx_msg void OnBnClickedAlignCheck();
	afx_msg void OnBnClickedCorrelateCheck();
	afx_msg LRESULT OnMovement(WPARAM w, LPARAM l);

public:

};
