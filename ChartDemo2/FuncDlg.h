#pragma once

enum CFuncDlg_Mode
{
	mode_add
	,mode_edit
};

// CFuncDlg dialog
class CChartDemo2Dlg;
class CPeriodicFunction;

class CFuncDlg : public CDialog
{
	DECLARE_DYNAMIC(CFuncDlg)

public:
	CFuncDlg(int sel, CPeriodicFunction* func, CChartDemo2Dlg* master, CFuncDlg_Mode mode, CWnd* pParent = NULL);   // standard constructor
	virtual ~CFuncDlg();

	// Dialog Data
	enum { IDD = IDD_FUNC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:  
	CChartDemo2Dlg* m_master;
	CPeriodicFunction* m_relatedFunction;
	CFuncDlg_Mode m_mode;
	int m_selected_line;

	CSliderCtrl m_a0_slider;
	CSliderCtrl m_f0_slider;
	CSliderCtrl m_dt_slider;

	CStatic m_a0_stvalue;
	CStatic m_f0_stvalue;
	CStatic m_dt_stvalue;

	int m_a0_value;
	int m_f0_value;
	int m_dt_value;

	int m_a0_func_value;
	int m_f0_func_value;
	double m_dt_func_value;
	
protected:
	LRESULT OnMovement(WPARAM w, LPARAM l);
	void InitSliders();
	void CountFunctionValues();
	void SetStaticData();
	virtual void OnOK() override;
};
