#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <map>
typedef std::map<int, int> pulse_points_t;
class CSignalProcessingDlg;

// EffectsDialog dialog

/** @brief  */
class EffectsDialog : public CDialog
{
	DECLARE_DYNAMIC(EffectsDialog)

public:
	EffectsDialog(CSignalProcessingDlg* master, CWnd* pParent = NULL);   // standard constructor
	virtual ~EffectsDialog();

// Dialog Data
	enum { IDD = IDD_EFFECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CSignalProcessingDlg* _master_dlg;

	CEdit m_point_edit;

	CListBox m_points_list;
	pulse_points_t _points_with_values;
	pulse_points_t _test_points;

	CButton m_add_point;
	CButton m_del_point;
	CButton m_clean_points;
	CButton m_apply_pulse_btn;
	
	CSliderCtrl _point_slider;
	int _point_slider_value;

	CStatic _point_value_static;

protected:
	afx_msg void OnBnClickedAddPoint();
	afx_msg void OnEnChangePointEdit();
	afx_msg void OnLbnSelchangePointList();
	afx_msg void OnBnClickedDeletePoint();
	afx_msg void OnBnClickedClearPoints();
	afx_msg void OnBnClickedWhiteNoiseBtn();
	afx_msg void OnBnClickedJumpNoiseBtn();
	afx_msg void OnBnClickedShiftNoiseBtn();
	afx_msg void OnBnClickedKnockApplyBtn();
	afx_msg void OnBnClickedMutualSaveBtn();
	afx_msg void OnBnClickedMutualClearBtn();
	afx_msg LRESULT OnMovement(WPARAM w, LPARAM l);

	// Включить/выключить кнопки управления
	// соответственно значениям списка точек
	void CheckItemsSelected();
	void SetSliderPosition(int pos);
	void SetStaticData();
public:
	
};
