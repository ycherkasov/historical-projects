// ChartDemo2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDemo2.h"
#include "ChartDemo2Dlg.h"
#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
#include "ChartSurfaceSerie.h"
#include "ChartGrid.h"
#include "ChartBarSerie.h"
#include "ChartAxisLabel.h"
#include "FuncDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CChartDemo2Dlg dialog


// ----------------------------------
CChartDemo2Dlg::CChartDemo2Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CChartDemo2Dlg::IDD, pParent)
, m_stat(this)
, m_align_val(FALSE)
, m_recountCorrelate(FALSE)
, m_serieType(harmonic_type)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


// ----------------------------------
CChartDemo2Dlg::~CChartDemo2Dlg()
{
}

// ----------------------------------
void CChartDemo2Dlg::OnDestroy()
{
	for(int i= 0 ; i < m_funcList.GetItemCount() ; i++ )
	{
		CPeriodicFunction* del = reinterpret_cast<CPeriodicFunction*>( m_funcList.GetItemData(i) ) ;
		delete del;
	}
}



// ----------------------------------
void CChartDemo2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_ChartCtrl);

	DDX_Control(pDX, IDC_CHECK_ALIGN, m_align_control);
	DDX_Control(pDX, IDC_FUNC_LIST, m_funcList);
	DDX_Control(pDX, IDC_AVG_STATIC2, m_average);
	DDX_Control(pDX, IDC_AVGS_STATIC2, m_sq_average);
	DDX_Control(pDX, IDC_DISP_STATIC2, m_dispersion);
	DDX_Control(pDX, IDC_DEV_STATIC2, m_deviation);
	DDX_Control(pDX, IDC_PROGRESS_STATIC, m_progressCaption);
	DDX_Control(pDX, IDC_COUNT_PROGRESS, m_recountProgress);

	DDX_Check(pDX, IDC_CHECK_ALIGN, m_align_val);
	DDX_Check(pDX, IDC_CHECK_CORRELATE, m_recountCorrelate);
	DDX_Control(pDX, IDC_CHECK_CORRELATE, m_autocorrButton);
	DDX_Control(pDX, IDC_BTN_ADDFUNC, m_addharmButton);
	DDX_Control(pDX, IDC_BTN_RMFUNC, m_remharmButton);
	DDX_Control(pDX, IDC_BTN_EDITFUNC, m_editharmButton);
	DDX_Control(pDX, IDC_MUTUALCORR_BUTTON, m_mutualcorrButton);
	DDX_Control(pDX, IDC_WHITENOISE_BUTTON, m_whitenoiseButton);
	DDX_Control(pDX, IDFFT, m_FTButton);
	DDX_Control(pDX, IDCANCEL, m_exitButton);
}

BEGIN_MESSAGE_MAP(CChartDemo2Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	//
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_ADDFUNC, &CChartDemo2Dlg::OnBnClickedBtnAddfunc)
	ON_BN_CLICKED(IDC_BTN_RMFUNC, &CChartDemo2Dlg::OnBnClickedBtnRmfunc)
	ON_BN_CLICKED(IDC_BTN_EDITFUNC, &CChartDemo2Dlg::OnBnClickedBtnEditfunc)
	ON_MESSAGE(WM_START_RECOUNT,&OnStartRecount)
	ON_MESSAGE(WM_HOP_RECOUNT,&OnHopRecount)
	ON_MESSAGE(WM_END_RECOUNT,&OnRecountComplete)
	ON_MESSAGE(WM_GRAPHIC_COMPLETE,&OnGraphicComplete)
	ON_BN_CLICKED(IDC_MUTUALCORR_BUTTON, &CChartDemo2Dlg::OnBnClickedMutualcorrButton)
	ON_BN_CLICKED(IDC_WHITENOISE_BUTTON, &CChartDemo2Dlg::OnBnClickedWhitenoiseButton)
	ON_BN_CLICKED(IDFFT, &CChartDemo2Dlg::OnBnClickedFft)
	ON_COMMAND(ID_FILE_EXIT, &CChartDemo2Dlg::OnFileExit)
	ON_COMMAND(ID_FILE_OPEN, &CChartDemo2Dlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CChartDemo2Dlg::OnFileSave)
END_MESSAGE_MAP()


// CChartDemo2Dlg message handlers

// ----------------------------------
BOOL CChartDemo2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// -- Init list ---
	// Get column width
	CRect r(0,0,0,0);
	m_funcList.GetClientRect(&r);
	int wd = r.Width()/3;

	// Describe columns
	LVCOLUMN col[3] = 
	{
		{LVCF_TEXT|LVCF_WIDTH, LVCFMT_LEFT, wd, L"A0", 0, 0, 0, 0}
		,{LVCF_TEXT|LVCF_WIDTH, LVCFMT_LEFT, wd, L"F0", 0, 0, 0, 1}
		,{LVCF_TEXT|LVCF_WIDTH, LVCFMT_LEFT, wd, L"dt", 0, 0, 0, 2}
	};

	// Insert it
	m_funcList.InsertColumn(0,&col[0]);
	m_funcList.InsertColumn(1,&col[1]);
	m_funcList.InsertColumn(2,&col[2]);

	// Add extra initialization here
	m_align_val = TRUE;
	m_recountCorrelate = TRUE;
	m_stat.SetRecountCorrelation(m_recountCorrelate);
	UpdateData(FALSE);

	// --- init menu ---
	m_mainMenu.LoadMenu(IDR_MAIN_MENU);
	SetMenu(&m_mainMenu);


	// ----------------- Init functions vector -----------------
	InitChart();

	m_recountProgress.SetRange32(0, SERIE_SIZE);
	ShowProgress(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


// ----------------------------------
void CChartDemo2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// ----------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChartDemo2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// ----------------------------------
void CChartDemo2Dlg::InitChart()
{
	// Disable the refresh of the control (avoid multiple refresh).
	m_ChartCtrl.EnableRefresh(false);

	//UpdateSerie();

	// Defines the different colors (back color, axes color, ...)

	COLORREF BackColor = RGB(0,50,0);
	COLORREF GridColor = RGB(0,180,0);
	COLORREF TextColor = RGB(0,180,0);

	if(m_serieType == harmonic_type)
	{
		BackColor = RGB(0,50,0);
		GridColor = RGB(0,180,0);
		TextColor = RGB(0,180,0);
	}
	else
	{
		BackColor = RGB(0,50,50);
		GridColor = RGB(0,180,180);
		TextColor = RGB(0,180,180);
	}

	// Specifies a sunken border for the control
	m_ChartCtrl.SetEdgeType(EDGE_SUNKEN);

	// Sets the color of the border and the back color
	m_ChartCtrl.SetBorderColor(TextColor);
	m_ChartCtrl.SetBackColor(BackColor);

	//Sets the color of the different elements of the bottom axis
	m_ChartCtrl.GetBottomAxis()->SetColor(TextColor);
	m_ChartCtrl.GetBottomAxis()->SetTextColor(TextColor);
	m_ChartCtrl.GetBottomAxis()->GetGrid()->SetColor(GridColor);

	// Sets the color of the different elements of the left axis
	m_ChartCtrl.GetLeftAxis()->SetColor(TextColor);
	m_ChartCtrl.GetLeftAxis()->SetTextColor(TextColor);
	m_ChartCtrl.GetLeftAxis()->GetGrid()->SetColor(GridColor);

	// Sets the color of the title, change the font to Times New Roman
	// and add a string
	m_ChartCtrl.GetTitle()->SetColor(TextColor);
	m_ChartCtrl.GetTitle()->SetFont(140,_T("Times New Roman"));
	m_ChartCtrl.GetTitle()->RemoveAll();
	m_ChartCtrl.GetTitle()->AddString(_T("Harmonics"));


	// Finally re-enable the refresh of the control. This will refresh the 
	// control if any refresh was still 'pending'.
	m_ChartCtrl.EnableRefresh(true);

}


// ----------------------------------
void CChartDemo2Dlg::UpdateSerie(bool autocorr, bool mutual_corr)
{
	m_ChartCtrl.RemoveAllSeries();

	COLORREF SerieColor1 = RGB(0,255,0);
	COLORREF SerieColor2 = RGB(0,0,255);
	COLORREF SerieColor3 = RGB(255,0,255);

	// Add a new series of type line to the control and add data to it
	m_pLine = (CChartLineSerie*)m_ChartCtrl.AddSerie(CChartSerie::stLineSerie);
	m_pLine->SetSeriesOrdering(CChartSerie::soNoOrdering);
	m_pLine->SetColor(SerieColor1);
	// Specifies that the points in the series are not ordered (needed to be able 
	// to draw an ellipse).

	if((m_recountCorrelate) && (autocorr))
	{
		m_pLineAutocorr = (CChartLineSerie*)m_ChartCtrl.AddSerie(CChartSerie::stLineSerie);
		m_pLineAutocorr->SetSeriesOrdering(CChartSerie::soNoOrdering);
		m_pLineAutocorr->SetColor(SerieColor2);
	}

	if((m_recountCorrelate) && (mutual_corr))
	{
		m_pLineMutualcorr = (CChartLineSerie*)m_ChartCtrl.AddSerie(CChartSerie::stLineSerie);
		m_pLineMutualcorr->SetSeriesOrdering(CChartSerie::soNoOrdering);
		m_pLineMutualcorr->SetColor(SerieColor3);
	}
	// Change the color of the line series

	double x =0. ,y= 0.;
	double max_a0 = 0.;
	int max_f0 = 0;
	for (int t=0 ; t < SERIE_SIZE ; t++)
	{
		m_stat.GetPeriodicFunction(t, x, y);

		if(y > max_a0)
			max_a0 = y;

		m_pLine->AddPoint(x, y);
		
		if((m_recountCorrelate) && (autocorr))
		{
			double self_cor = m_stat.GetSelfCorreleted(t);
			m_pLineAutocorr->AddPoint(x,self_cor);
		}
		if((m_recountCorrelate) && (mutual_corr))
		{
			double mut_cor = m_stat.GetMutualCorreleted(t);
			m_pLineMutualcorr->AddPoint(x,mut_cor);
		}			
	}

	// Sets the min and max values of the bottom and left axis to -15 -> 15
	if(m_align_val)
	{
		//m_ChartCtrl.GetBottomAxis()->SetMinMax(0,10.0/max_f0);
		m_ChartCtrl.GetBottomAxis()->SetMinMax(0,1.0);
		m_ChartCtrl.GetLeftAxis()->SetMinMax(-max_a0 - 5 , max_a0 + 5);
	}

	SetStaticData();
}


// ----------------------------------
void CChartDemo2Dlg::UpdateFurie()
{
	m_ChartCtrl.RemoveAllSeries();

	COLORREF SerieColor1 = RGB(0,255,0);
	COLORREF SerieColor2 = RGB(0,0,255);
	COLORREF SerieColor3 = RGB(255,0,255);

	// Add a new series of type line to the control and add data to it
	m_pLineFT = (CChartLineSerie*)m_ChartCtrl.AddSerie(CChartSerie::stLineSerie);
	m_pLineFT->SetSeriesOrdering(CChartSerie::soNoOrdering);
	m_pLineFT->SetColor(SerieColor1);
	// Specifies that the points in the series are not ordered (needed to be able 
	// to draw an ellipse).



	double x = 0. ,y = 0.;
	for (int t=0 ; t < SERIE_SIZE/2 ; t++)
	{
		m_stat.GetFurieTransform(t, x, y);
		m_pLineFT->AddPoint(x, y);
	}

	// Sets the min and max values of the bottom and left axis to -15 -> 15
	if(m_align_val)
	{
		m_ChartCtrl.GetBottomAxis()->SetMinMax(0, 500);
		m_ChartCtrl.GetLeftAxis()->SetMinMax(-300 , 300);
	}

}


void CChartDemo2Dlg::AddLine(int a0, int f0, double dt)
{
	LVITEM lv_a0 = {0} ;
	LVITEM lv_f0 = {0} ;
	LVITEM lv_dt = {0} ;
	UINT item_mask = LVIF_TEXT;
	int nItem = m_funcList.GetItemCount();

	lv_dt.mask = lv_f0.mask = lv_a0.mask = item_mask;
	lv_dt.iItem = lv_f0.iItem = lv_a0.iItem = nItem;
	
	lv_a0.iItem = nItem;
	lv_f0.iItem = nItem;
	lv_dt.iItem = nItem;

	lv_a0.iSubItem = 0;
	lv_f0.iSubItem = 1;
	lv_dt.iSubItem = 2;

	wchar_t a0str[255] = {0};
	wchar_t f0str[255] = {0};
	wchar_t dtstr[255] = {0};
	_itow_s(a0,a0str,255,10);
	_itow_s(f0,f0str,255,10);
	swprintf_s(dtstr,255,L"%.4f",dt);

	lv_a0.pszText = a0str;
	lv_f0.pszText = f0str;
	lv_dt.pszText = dtstr;

	int ret = -1;
	ret = m_funcList.InsertItem(&lv_a0);
	ret = m_funcList.SetItem(&lv_f0);
	ret = m_funcList.SetItem(&lv_dt);

	CPeriodicFunction* newPeriodic = new CPeriodicFunction(SERIE_SIZE,a0,f0,dt);
	DWORD_PTR itemData = reinterpret_cast<DWORD_PTR>(newPeriodic);
	m_funcList.SetItemData(nItem, itemData);

	m_stat.SetRecountCorrelation(m_recountCorrelate);
	m_stat.AddSerie(newPeriodic);
}

void CChartDemo2Dlg::SetLine(int nLine, int a0, int f0, double dt)
{
	LVITEM lv_a0 = {0} ;
	LVITEM lv_f0 = {0} ;
	LVITEM lv_dt = {0} ;
	UINT item_mask = LVIF_TEXT;
	int nItem = nLine;

	lv_dt.mask = lv_f0.mask = lv_a0.mask = item_mask;
	lv_dt.iItem = lv_f0.iItem = lv_a0.iItem = nItem;

	lv_a0.iItem = nItem;
	lv_f0.iItem = nItem;
	lv_dt.iItem = nItem;

	lv_a0.iSubItem = 0;
	lv_f0.iSubItem = 1;
	lv_dt.iSubItem = 2;

	wchar_t a0str[255] = {0};
	wchar_t f0str[255] = {0};
	wchar_t dtstr[255] = {0};
	_itow_s(a0,a0str,255,10);
	_itow_s(f0,f0str,255,10);
	swprintf_s(dtstr,255,L"%.4f",dt);

	lv_a0.pszText = a0str;
	lv_f0.pszText = f0str;
	lv_dt.pszText = dtstr;

	int ret = -1;
	ret = m_funcList.SetItem(&lv_a0);
	ret = m_funcList.SetItem(&lv_f0);
	ret = m_funcList.SetItem(&lv_dt);

	DWORD_PTR itemData = m_funcList.GetItemData(nItem);
	CPeriodicFunction* Periodic = reinterpret_cast<CPeriodicFunction*>(itemData);
	Periodic->m_a0 = a0;
	Periodic->m_f0 = f0;
	Periodic->m_dt = dt;
	m_stat.SetRecountCorrelation(m_recountCorrelate);
	m_stat.SetSerie();
}



void CChartDemo2Dlg::DeleteLine(int nItem)
{

	DWORD_PTR itemData = m_funcList.GetItemData(nItem);
	CPeriodicFunction* Periodic = reinterpret_cast<CPeriodicFunction*>(itemData);
	m_stat.SetRecountCorrelation(m_recountCorrelate);
	m_stat.DeleteSerie(Periodic);
	m_funcList.DeleteItem(nItem);
	delete Periodic;
}

void CChartDemo2Dlg::SetStaticData()
{
	wchar_t str[255] = {0};

	swprintf( str, 255, L"%.4f", m_stat.Average() );
	m_average.SetWindowText( str ) ;

	swprintf( str, 255, L"%.4f", m_stat.AverageSquare() );
	m_sq_average.SetWindowText( str ) ;

	swprintf( str, 255, L"%.4f", m_stat.Dispersion() );
	m_dispersion.SetWindowText( str ) ;

	swprintf( str, 255, L"%.4f", m_stat.Deviation() );
	m_deviation.SetWindowText( str ) ;
}


void CChartDemo2Dlg::ShowProgress(BOOL show)
{
	if(show)
	{
		m_progressCaption.ShowWindow(SW_SHOW);
		m_recountProgress.ShowWindow(SW_SHOW);

		m_align_control.EnableWindow(FALSE);
		m_autocorrButton.EnableWindow(FALSE);
		m_addharmButton.EnableWindow(FALSE);
		m_remharmButton.EnableWindow(FALSE);
		m_editharmButton.EnableWindow(FALSE);
		m_mutualcorrButton.EnableWindow(FALSE);
		m_whitenoiseButton.EnableWindow(FALSE);
		m_FTButton.EnableWindow(FALSE);
		m_exitButton.EnableWindow(FALSE);
	}
	else
	{
		m_progressCaption.ShowWindow(SW_HIDE);
		m_recountProgress.ShowWindow(SW_HIDE);

		m_align_control.EnableWindow(TRUE);
		m_autocorrButton.EnableWindow(TRUE);
		m_addharmButton.EnableWindow(TRUE);
		m_remharmButton.EnableWindow(TRUE);
		m_editharmButton.EnableWindow(TRUE);
		m_mutualcorrButton.EnableWindow(TRUE);
		m_whitenoiseButton.EnableWindow(TRUE);
		m_FTButton.EnableWindow(TRUE);
		m_exitButton.EnableWindow(TRUE);
	}
}


LRESULT CChartDemo2Dlg::OnStartRecount(WPARAM w, LPARAM l)
{
	ShowProgress(TRUE);
	return 0L;
}


LRESULT CChartDemo2Dlg::OnHopRecount(WPARAM w, LPARAM l)
{
	m_recountProgress.SetPos(static_cast<int>(w));
	return 0L;
}

LRESULT CChartDemo2Dlg::OnGraphicComplete(WPARAM w, LPARAM l)
{
	UpdateSerie(false, false);
	return 0L;
}


LRESULT CChartDemo2Dlg::OnRecountComplete(WPARAM w, LPARAM l)
{
	// WPARAM is BOOL for mutual correlation
	ShowProgress(FALSE);
	UpdateSerie(true, (w)?true:false);
	return 0L;
}



void CChartDemo2Dlg::OnBnClickedBtnAddfunc()
{
	CFuncDlg fd(-1, 0, (CChartDemo2Dlg*)this, mode_add);
	fd.DoModal();
}


void CChartDemo2Dlg::OnBnClickedBtnRmfunc()
{
	int selected = -1;
	POSITION pos = m_funcList.GetFirstSelectedItemPosition();
	if(pos)
		selected = m_funcList.GetNextSelectedItem(pos);
	DeleteLine(selected);
}


void CChartDemo2Dlg::OnBnClickedBtnEditfunc()
{
	int selected = -1;
	POSITION pos = m_funcList.GetFirstSelectedItemPosition();
	if(pos)
		selected = m_funcList.GetNextSelectedItem(pos);
		DWORD_PTR related = m_funcList.GetItemData(selected);
	CPeriodicFunction* pf = reinterpret_cast<CPeriodicFunction*>(related);
	CFuncDlg fd(selected, pf, (CChartDemo2Dlg*)this, mode_edit);
	fd.DoModal();
}

void CChartDemo2Dlg::OnBnClickedMutualcorrButton()
{
	m_stat.SaveCorrelation();
}

void CChartDemo2Dlg::OnBnClickedWhitenoiseButton()
{
	m_stat.WhiteNoise();
}

void CChartDemo2Dlg::OnBnClickedFft()
{
	m_stat.FurieTransform();	
	if(m_serieType == harmonic_type)
	{
		
		m_serieType = furie_type;
		InitChart();
		UpdateFurie();
		m_FTButton.SetWindowText(L"нот");
		m_ChartCtrl.GetTitle()->RemoveAll();
		m_ChartCtrl.GetTitle()->AddString(_T("Furie Transformation"));
	}
	else
	{
		//m_stat.FurieTransformReverse();
		m_serieType = harmonic_type;
		InitChart();
		UpdateSerie(false, false);
		m_FTButton.SetWindowText(L"дот");
		m_ChartCtrl.GetTitle()->RemoveAll();
		m_ChartCtrl.GetTitle()->AddString(_T("Harmonics"));

	}
}

void CChartDemo2Dlg::OnFileExit()
{
	OnOK();
	// TODO: Add your command handler code here
}

void CChartDemo2Dlg::OnFileOpen()
{
	// TODO: Add your command handler code here
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name
	HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = this->GetSafeHwnd();
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Osciloscope data files\0*.DAT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE) {

	}


}

void CChartDemo2Dlg::OnFileSave()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name
	HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = this->GetSafeHwnd();
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Osciloscope data files\0*.DAT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn)==TRUE) {
		__asm nop
	}
}
