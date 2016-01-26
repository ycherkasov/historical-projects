// SignalProcessingDlg.cpp : implementation file
//

#include "stdafx.h"

// Application
#include "SignalProcessing.h"
#include "SignalProcessingDlg.h"
#include "HarmonicFile.h"
#include "PulseAttenuation.h"
#include "SignalFilter.h"
#include "SerieSettings.h"

// Chart Control
#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartPointsSerie.h"
#include "ChartCtrl/ChartSurfaceSerie.h"
#include "ChartCtrl/ChartGrid.h"
#include "ChartCtrl/ChartBarSerie.h"
#include "ChartCtrl/ChartAxisLabel.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// ------------------ CSignalProcessingDlg dialog ------------------ 

CSignalProcessingDlg::CSignalProcessingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignalProcessingDlg::IDD, pParent)
	, _statistic(this)
	, _harmonic_dialog(this)
	, _settings_dialog(this)
	, _effect_dialog(this)
	, _filters_dilaog(this)
	, _main_signal( &_signalCtrl, RGB(0,255,0) )
	, _auto_correlation( &_signalCtrl, RGB(0,0,255) )
	, _mutual_correlation( &_signalCtrl, RGB(255,0,255) )
	, _signal_spectrum( &_spectrumCtrl, RGB(255,0,0) )
	, _weight_function( &_weightCtrl, RGB(0,0,255) )
	, _tmp_function( &_weightCtrl, RGB(255,0,0) )
	, _is_auto_correlate(false)
	, _is_mutual_correlate(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	dlgs[HarmonicsDlgSheet] = &_harmonic_dialog;
	dlgs[EffectsDlgSheet] = &_effect_dialog;
	dlgs[FiltersDlgSheet] = &_filters_dilaog;
	dlgs[StatisticDlgSheet] = &_statistic_dialog;
	dlgs[SettingsDlgSheet] = &_settings_dialog;
}

void CSignalProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIGNAL_CHART, _signalCtrl);
	DDX_Control(pDX, IDC_SPECTRUM_CHART, _spectrumCtrl);
	DDX_Control(pDX, IDC_WEIGHT_CHART, _weightCtrl);
	DDX_Control(pDX, IDC_SETTINGS_TAB, _settingsTab);
	DDX_Control(pDX, IDC_RECOUNT_STATIC, _recount_static);
	DDX_Control(pDX, IDC_RECOUNT_PROGRESS, _recount_prgress);
}

BEGIN_MESSAGE_MAP(CSignalProcessingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_SETTINGS_TAB, &CSignalProcessingDlg::OnTcnSelchangeSettingsTab)

	ON_MESSAGE(WM_START_RECOUNT,&OnStartRecount)
	ON_MESSAGE(WM_HOP_RECOUNT,&OnHopRecount)
	ON_MESSAGE(WM_END_RECOUNT,&OnRecountComplete)
	ON_MESSAGE(WM_END_FT,&OnFTComplete)

	ON_COMMAND(ID_FILE_OPEN, &CSignalProcessingDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CSignalProcessingDlg::OnFileSave)
	ON_COMMAND(ID_FILE_EXIT, &CSignalProcessingDlg::OnFileExit)
	ON_COMMAND(ID_HELP_ABOUT, &CSignalProcessingDlg::OnHelpAbout)
END_MESSAGE_MAP()


// CSignalProcessingDlg message handlers

BOOL CSignalProcessingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SerieSettings::SetSerieSize(1000);

	ShowWindow(SW_MAXIMIZE);

	// Add extra initialization here
	InitTabs();
	InitCharts();
	CreateChildDialogs();
	Sizing();
	
	_statistic.SetMasterFunction( _main_signal.GetFunctionSerie() );

	_mainMenu.LoadMenu(IDR_MAIN_MENU);
	SetMenu(&_mainMenu);

	_recount_prgress.SetRange32(0, SerieSettings::SerieSize());
	ShowProgress(FALSE);

	// show spectrum chart
	ChangeSignalInformationChart(FALSE);

	// Set signal align
	_signal_spectrum.SetAlign(true);
	_weight_function.SetAlign(true);
	//_tmp_function.SetAlign(true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSignalProcessingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSignalProcessingDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSignalProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSignalProcessingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	const int border = 10;

	// Разместить контролы
	CRect r(0,0,0,0);
	GetClientRect(&r);
	int centerW = r.Width()/2;
	int bottomDlg = r.Height()/3;
	int centerH = r.Height()*(3./8.);

	// Settings dialogs tab placement
	_settingsRect.left = r.left + border;
	_settingsRect.top = r.top + border;
	_settingsRect.right = r.right - border;
	_settingsRect.bottom = bottomDlg - border/2;

	// Recount static placement
	_recountStaticRect.left = _settingsRect.left;
	_recountStaticRect.top = _settingsRect.bottom + border/2;
	_recountStaticRect.right = _recountStaticRect.left + 200;
	_recountStaticRect.bottom = _recountStaticRect.top + 20;

	// Recount progress bar placement
	_recountProgressRect.left = 200 + border;
	_recountProgressRect.top = _settingsRect.bottom + border/2;
	_recountProgressRect.right = _settingsRect.right;
	_recountProgressRect.bottom = _recountStaticRect.top + 20;

	// Child tab dialogs placement
	/* Magic */
	_childDialogRect.left = _settingsRect.left - 8;
	_childDialogRect.top = _settingsRect.top + 12;
	_childDialogRect.right = _settingsRect.right - 14;
	_childDialogRect.bottom = _settingsRect.bottom - 14;

	// Signal chart control placement
	_signalChartRect.left = r.left + border;
	_signalChartRect.top = centerH + border/2;
	_signalChartRect.right = centerW - border/2;
	_signalChartRect.bottom = r.bottom - border;

	// Spectrum chart control placement
	_spectrumChartRect.left = centerW + border/2;
	_spectrumChartRect.top = centerH + border/2;
	_spectrumChartRect.right = r.right - border;
	_spectrumChartRect.bottom = r.bottom - border;

	Sizing();
}

void CSignalProcessingDlg::InitTabs()
{
	int index = 0;
	InitTab(index++, L"Гармоники");
	InitTab(index++, L"Эффекты");
	InitTab(index++, L"Фильтры");
	InitTab(index++, L"Статистика");
	InitTab(index++, L"Настройки");
}

void CSignalProcessingDlg::InitTab(UINT index, LPWSTR caption)
{
	TCITEM tItem = {0};
	tItem.mask = TCIF_TEXT;
	tItem.pszText = caption;
	_settingsTab.InsertItem(index,&tItem);
}

void CSignalProcessingDlg::InitChart(CChartCtrl& ctrl, LPCTSTR title, ChartColorScheme& cs)
{
	// Disable the refresh of the control (avoid multiple refresh).
	ctrl.EnableRefresh(false);

	// Specifies a sunken border for the control
	ctrl.SetEdgeType(EDGE_SUNKEN);

	// Sets the color of the border and the back color
	ctrl.SetBorderColor(cs.TextColor);
	ctrl.SetBackColor(cs.BackColor);

	//Sets the color of the different elements of the bottom axis
	ctrl.GetBottomAxis()->SetColor(cs.TextColor);
	ctrl.GetBottomAxis()->SetTextColor(cs.TextColor);
	ctrl.GetBottomAxis()->GetGrid()->SetColor(cs.GridColor);

	// Sets the color of the different elements of the left axis
	ctrl.GetLeftAxis()->SetColor(cs.TextColor);
	ctrl.GetLeftAxis()->SetTextColor(cs.TextColor);
	ctrl.GetLeftAxis()->GetGrid()->SetColor(cs.GridColor);

	// Sets the color of the title, change the font to Times New Roman
	// and add a string
	ctrl.GetTitle()->SetColor(cs.TextColor);
	ctrl.GetTitle()->SetFont(140,_T("Times New Roman"));
	ctrl.GetTitle()->RemoveAll();
	ctrl.GetTitle()->AddString(title);


	// Finally re-enable the refresh of the control. This will refresh the 
	// control if any refresh was still 'pending'.
	ctrl.EnableRefresh(true);
}

void CSignalProcessingDlg::InitCharts()
{
	ChartColorScheme sc = {RGB(0,50,0), RGB(0,180,0), RGB(0,180,0)};
	InitChart(_signalCtrl, L"Сигнал",  sc);
	
	sc.BackColor = RGB(0,50,50);
	sc.GridColor = RGB(0,180,180);
	sc.TextColor = RGB(0,180,180);
	InitChart( _spectrumCtrl, L"Спектр", sc);

	sc.BackColor = RGB(50,0,50);
	sc.GridColor = RGB(180,0,180);
	sc.TextColor = RGB(180,0,180);
	InitChart( _weightCtrl, L"Весовая функция", sc);
}


void CSignalProcessingDlg::CreateChildDialogs()
{
	CRect r(0,0,0,0);
	GetClientRect(&r);

	_harmonic_dialog.Create(IDD_HARMONICS, (CWnd*)&_settingsTab );
	_effect_dialog.Create(IDD_EFFECTS, (CWnd*)&_settingsTab );
	_filters_dilaog.Create(IDD_FILTERS, (CWnd*)&_settingsTab );
	_statistic_dialog.Create(IDD_STAT, (CWnd*)&_settingsTab );
	_settings_dialog.Create(IDD_SETTINGS, (CWnd*)&_settingsTab );

	_harmonic_dialog.ShowWindow(SW_SHOW);
}

void CSignalProcessingDlg::Sizing()
{
	WindowPlacement( static_cast<CWnd*>(&_settingsTab), _settingsRect);
	WindowPlacement( static_cast<CWnd*>(&_signalCtrl), _signalChartRect);
	WindowPlacement( static_cast<CWnd*>(&_spectrumCtrl), _spectrumChartRect);
	WindowPlacement( static_cast<CWnd*>(&_weightCtrl), _spectrumChartRect);
	WindowPlacement( static_cast<CWnd*>(&_harmonic_dialog), _childDialogRect);
	WindowPlacement( static_cast<CWnd*>(&_effect_dialog), _childDialogRect);
	WindowPlacement( static_cast<CWnd*>(&_filters_dilaog), _childDialogRect);
	WindowPlacement( static_cast<CWnd*>(&_statistic_dialog), _childDialogRect);
	WindowPlacement( static_cast<CWnd*>(&_settings_dialog), _childDialogRect);
	WindowPlacement( static_cast<CWnd*>(&_recount_prgress), _recountProgressRect);
	WindowPlacement( static_cast<CWnd*>(&_recount_static), _recountStaticRect);
}

void CSignalProcessingDlg::WindowPlacement(CWnd* wnd, RECT& rect)
{
	if(!wnd->GetSafeHwnd())
		return;

	wnd->SetWindowPos(NULL, rect.left
		, rect.top
		, rect.right - rect.left
		, rect.bottom - rect.top
		, SWP_NOZORDER);
}

void CSignalProcessingDlg::UpdateSerie()
{
	_signalCtrl.RemoveAllSeries();

	try
	{
		_main_signal.UpdateSerie();
		
		if(_is_auto_correlate)
			_auto_correlation.UpdateSerie();

		if(_is_mutual_correlate)
			_mutual_correlation.UpdateSerie();
	}
	catch (const std::exception& e)
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
		return;
	}
}

void CSignalProcessingDlg::UpdateSpectrum()
{
	_spectrumCtrl.RemoveAllSeries();

	try
	{
		_signal_spectrum.UpdateSerie();
	}
	catch (const std::exception& e)
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
		return;
	}
}

void CSignalProcessingDlg::UpdateWeight()
{
	_weightCtrl.RemoveAllSeries();

	try
	{
		_weight_function.UpdateSerie();
		_tmp_function.UpdateSerie();
	}
	catch (const std::exception& e)
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
		return;
	}
}

void CSignalProcessingDlg::SetSignalAlign(BOOL algn)
{
	_main_signal.SetAlign(algn?true:false);
}

void CSignalProcessingDlg::SetAutocorrelation(BOOL autocorr)
{
	bool b = (autocorr)?true:false;
	_is_auto_correlate = b;
	_is_mutual_correlate = b;
	_statistic.SetCorrelationRecount(b);
}

void CSignalProcessingDlg::Recount()
{
	UINT addr = -1;
	_beginthreadex(NULL, NULL, FunctionStatistics::RecountStatistics, &_statistic ,NULL, &addr);
}

void CSignalProcessingDlg::GenerateAttenuationSignal(const pulse_points_t& att_settings)
{
	PulseAttenuation::GeterateAttenuationSerie(_main_signal.GetFunctionSerie(), att_settings);
}

void CSignalProcessingDlg::GenerateFilteredSignal(const filter_t& filter)
{
	SignalFilter::FilterSerie( _main_signal.GetFunctionSerie()
		, _weight_function.GetFunctionSerie()
		, _tmp_function.GetFunctionSerie()
		, _filters_dilaog.GetWeightFunctionCount()
		, filter );
	UpdateWeight();
}

void CSignalProcessingDlg::CleanJumpNoise()
{
	for(int i = 0 ; i < 5 ; i++)
		SignalFilter::FilterJumps( _main_signal.GetFunctionSerie(), _statistic.Deviation() );
	FinishRecount(false);
}

void CSignalProcessingDlg::CleanBiasNoise()
{
	SignalFilter::FilterShifts( _main_signal.GetFunctionSerie(), _statistic.Average() );
	Recount();
}

void CSignalProcessingDlg::RecountSignal(const ValuesSet& vals)
{
	_main_signal.GetFunctionSerie()->ResetWithSet(vals);
	Recount();
}

void CSignalProcessingDlg::FinishRecount(bool also_correlate)
{
	ValuesSet vs;
	_statistic.FillSelfCorrelation( vs );
	_auto_correlation.GetFunctionSerie()->ResetWithSet(vs);

	// param for mutual correlation
	if(also_correlate)
	{
		_statistic.FillMutualCorrelation( vs );
		_mutual_correlation.GetFunctionSerie()->ResetWithSet(vs);
	}

	UpdateSerie();

	_statistic.FurieTransform();

	// output statistic here
	_statistic_dialog.SetStaticData( _statistic.Average()
		, _statistic.AverageSq()
		, _statistic.Dispersion()
		, _statistic.Deviation() );
}

void CSignalProcessingDlg::ChangeSignalInformationChart(BOOL is_weight)
{
	_spectrumCtrl.ShowWindow(!is_weight);
	_weightCtrl.ShowWindow(is_weight);
}

LRESULT CSignalProcessingDlg::OnStartRecount(WPARAM w, LPARAM l)
{
	_recount_prgress.SetRange32(0, w);
	ShowProgress(TRUE);
	return 0L;
}

LRESULT CSignalProcessingDlg::OnHopRecount(WPARAM w, LPARAM l)
{
	_recount_prgress.SetPos(static_cast<int>(w));
	return 0L;
}

LRESULT CSignalProcessingDlg::OnRecountComplete(WPARAM w, LPARAM l)
{
	FinishRecount(w?true:false);
	ShowProgress(FALSE);
	return 0L;
}

LRESULT CSignalProcessingDlg::OnFTComplete(WPARAM w, LPARAM l)
{
	_signal_spectrum.GetFunctionSerie()->ResetWithSet( _statistic.GetFTValues() );
	UpdateSpectrum();
	ShowProgress(FALSE);
	return 0L;
}

void CSignalProcessingDlg::ShowProgress(BOOL show)
{
	_settingsTab.EnableWindow(!show);

	if(show)
	{
		_recount_prgress.ShowWindow(SW_SHOW);
		_recount_static.ShowWindow(SW_SHOW);
	}
	else
	{
		_recount_prgress.ShowWindow(SW_HIDE);
		_recount_static.ShowWindow(SW_HIDE);
	}
}

void CSignalProcessingDlg::SaveMutualCorrelation()
{
	_statistic.SaveCorrelation();
}

void CSignalProcessingDlg::ClearMutualCorrelation()
{
	_statistic.ClearCorrelation();
}

int CSignalProcessingDlg::GetWeightFunctionCount()
{
	return _filters_dilaog.GetWeightFunctionCount();
}

void CSignalProcessingDlg::ClearSignal()
{
	_signalCtrl.RemoveAllSeries();
	_spectrumCtrl.RemoveAllSeries();
	_weightCtrl.RemoveAllSeries();

	_main_signal.GetFunctionSerie()->Clear();
	_auto_correlation.GetFunctionSerie()->Clear();
	_mutual_correlation.GetFunctionSerie()->Clear();
	_signal_spectrum.GetFunctionSerie()->Clear();
	_weight_function.GetFunctionSerie()->Clear();
	_tmp_function.GetFunctionSerie()->Clear();
	
}

void CSignalProcessingDlg::OnTcnSelchangeSettingsTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = TabCtrl_GetCurSel( _settingsTab.GetSafeHwnd() );
	*pResult = 0;


	// TODO : for_each
	//for_each(dlgs, dlgs + DlgSheetCount, ShowWindow)

	_harmonic_dialog.ShowWindow(SW_HIDE);
	_effect_dialog.ShowWindow(SW_HIDE);
	_filters_dilaog.ShowWindow(SW_HIDE);
	_statistic_dialog.ShowWindow(SW_HIDE);
	_settings_dialog.ShowWindow(SW_HIDE);

	dlgs[sel]->ShowWindow(SW_SHOW);
}

void CSignalProcessingDlg::OnFileOpen()
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name

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
	ofn.lpstrFilter = L"Oscilloscope data files\0*.DAT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if ( GetOpenFileName(&ofn) == TRUE )
	{
		_harmonic_dialog.CleanHarmonics();
		ValuesSet vs;
		HarmonicFile::FromFile(ofn.lpstrFile, vs);
		RecountSignal(vs);
	}
}

void CSignalProcessingDlg::OnFileSave()
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name

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
	ofn.lpstrFilter = L"Oscilloscope data files\0*.DAT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn)==TRUE) {
		HarmonicFile::ToFile(ofn.lpstrFile, _main_signal.GetFunctionSerie()->GetValuesSet());
	}
}

void CSignalProcessingDlg::OnFileExit()
{
	OnOK();
}

void CSignalProcessingDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
