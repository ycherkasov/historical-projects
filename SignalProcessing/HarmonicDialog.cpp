// HarmonicDialog.cpp : implementation file
//

#include "stdafx.h"

#include "SignalProcessing.h"
#include "HarmonicDialog.h"
#include "SignalProcessingDlg.h"

// HarmonicDialog dialog

IMPLEMENT_DYNAMIC(HarmonicDialog, CDialog)

// ---------------  --------------- 
HarmonicDialog::HarmonicDialog(CSignalProcessingDlg* master, CWnd* pParent /*=NULL*/)
	: CDialog(HarmonicDialog::IDD, pParent)
	, _master(master)
	, m_a0_value(0)
	, m_f0_value(0)
	
{

}

// ---------------  --------------- 
HarmonicDialog::~HarmonicDialog()
{
}

// ---------------  --------------- 
void HarmonicDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FUNC_LIST, m_funcList);

	DDX_Control(pDX, IDC_SLIDER_A0, m_a0_slider);
	DDX_Control(pDX, IDC_SLIDER_F0, m_f0_slider);

	DDX_Control(pDX, IDC_STATIC_F0, m_f0_stvalue);
	DDX_Control(pDX, IDC_STATIC_A0, m_a0_stvalue);

	DDX_Slider(pDX, IDC_SLIDER_A0, m_a0_value);
	DDX_Slider(pDX, IDC_SLIDER_F0, m_f0_value);
}

// ---------------  --------------- 
BEGIN_MESSAGE_MAP(HarmonicDialog, CDialog)
	ON_BN_CLICKED(IDC_ADD_HARMONIC, &HarmonicDialog::OnBnClickedAddHarmonic)
	ON_BN_CLICKED(IDC_DEL_HARMONIC, &HarmonicDialog::OnBnClickedDelHarmonic)
	ON_BN_CLICKED(IDC_CLEAN_HARMONIC, &HarmonicDialog::OnBnClickedCleanHarmonic)

	ON_MESSAGE(WM_VSCROLL, OnMovement )
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FUNC_LIST, &HarmonicDialog::OnLvnItemchangedFuncList)
	
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// ---------------  --------------- 
BOOL HarmonicDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitSliders();
	EnableSliders(FALSE);
	SetSlidersPos(0, 0);
	InitHarmonicsList();
	return TRUE;
}

// ---------------  --------------- 
void HarmonicDialog::InitHarmonicsList()
{
	// -- Init list ---
	// Get column width
	CRect r(0,0,0,0);
	m_funcList.GetClientRect(&r);
	int wd = r.Width()/2;

	// Describe columns
	LVCOLUMN col[2] = 
	{
		 {LVCF_TEXT|LVCF_WIDTH, LVCFMT_LEFT, wd, L"A0", 0, 0, 0, 0}
		,{LVCF_TEXT|LVCF_WIDTH, LVCFMT_LEFT, wd, L"F0", 0, 0, 0, 1}
	};

	// Insert it
	m_funcList.InsertColumn(0,&col[0]);
	m_funcList.InsertColumn(1,&col[1]);

}

// ---------------  --------------- 
void HarmonicDialog::InitSliders()
{
	m_a0_slider.SetRange(0, 50);  //*1
	m_f0_slider.SetRange(0, 100); //*5
}

// ---------------  --------------- 
void HarmonicDialog::OnDestroy()
{
	CleanListItemData();
	CDialog::OnDestroy();
}

void HarmonicDialog::CleanHarmonics()
{
	CleanListItemData();
	m_funcList.DeleteAllItems();
	SetSlidersPos(0, 0);
	EnableSliders(FALSE);
	_harmonics.clear();
}


// ---------------  --------------- 
void HarmonicDialog::AddLine()
{
	int a0 = 0, f0 = 0;
	LVITEM lv_a0 = {0};
	LVITEM lv_f0 = {0};
	UINT item_mask = LVIF_TEXT;
	int nItem = m_funcList.GetItemCount();

	lv_f0.mask = lv_a0.mask = item_mask;
	lv_f0.iItem = lv_a0.iItem = nItem;

	lv_a0.iItem = nItem;
	lv_f0.iItem = nItem;

	lv_a0.iSubItem = 0;
	lv_f0.iSubItem = 1;

	wchar_t a0str[255] = {0};
	wchar_t f0str[255] = {0};
	_itow_s(a0,a0str,255,10);
	_itow_s(f0,f0str,255,10);

	lv_a0.pszText = a0str;
	lv_f0.pszText = f0str;

	int ret = -1;
	ret = m_funcList.InsertItem(&lv_a0);
	ret = m_funcList.SetItem(&lv_f0);

	AddListItemData(nItem, a0, f0);
}

// ---------------  --------------- 
void HarmonicDialog::SetLine(int nLine)
{
	if(nLine == -1)
		return;

	LVITEM lv_a0 = {0} ;
	LVITEM lv_f0 = {0} ;
	
	UINT item_mask = LVIF_TEXT;
	int nItem = nLine;

	lv_f0.mask = lv_a0.mask = item_mask;
	lv_f0.iItem = lv_a0.iItem = nItem;

	lv_a0.iItem = nItem;
	lv_f0.iItem = nItem;

	lv_a0.iSubItem = 0;
	lv_f0.iSubItem = 1;

	wchar_t a0str[255] = {0};
	wchar_t f0str[255] = {0};
	_itow_s(m_a0_func_value,a0str,255,10);
	_itow_s(m_f0_func_value,f0str,255,10);

	lv_a0.pszText = a0str;
	lv_f0.pszText = f0str;

	int ret = -1;
	ret = m_funcList.SetItem(&lv_a0);
	ret = m_funcList.SetItem(&lv_f0);

	DWORD_PTR itemData = m_funcList.GetItemData(nItem);
	ListHarmonicData* Periodic = reinterpret_cast<ListHarmonicData*>(itemData);
	Periodic->a0 = m_a0_func_value;
	Periodic->f0 = m_f0_func_value;
}

// ---------------  --------------- 
int HarmonicDialog::GetSelectedLine()
{
	POSITION pos = m_funcList.GetFirstSelectedItemPosition();
	if(pos)
		return m_funcList.GetNextSelectedItem(pos);
	return -1;
}

// ---------------  --------------- 
void HarmonicDialog::SetSlidersPos(int a_pos, int f_pos)
{
	m_a0_slider.SetPos(a_pos);
	m_f0_slider.SetPos(f_pos);
	UpdateControls();
}

// ---------------  --------------- 
void HarmonicDialog::EnableSliders(BOOL enable)
{
	m_a0_slider.EnableWindow(enable);
	m_f0_slider.EnableWindow(enable);
}

// ---------------  --------------- 
void HarmonicDialog::DeleteLine(int nItem)
{
	DeleteListItemData(nItem);
	m_funcList.DeleteItem(nItem);
}

// ---------------  --------------- 
void HarmonicDialog::OnBnClickedAddHarmonic()
{
	AddLine();
}

// ---------------  --------------- 
void HarmonicDialog::OnBnClickedDelHarmonic()
{
	int selected = GetSelectedLine();
	DeleteLine(selected);
}

// ---------------  --------------- 
void HarmonicDialog::OnBnClickedCleanHarmonic()
{
	CleanHarmonics();
	_master->ClearSignal();
}

// ---------------  --------------- 
LRESULT HarmonicDialog::OnMovement(WPARAM w, LPARAM l)
{
	UpdateControls();
	if(w == TB_ENDTRACK)
	{
		// пересчитать функцию
		// пересчитать ф-ции корреляции
		if(m_a0_value && m_f0_value)
			RecountHarmonics();
	}
	return 0l;
}

// ---------------  --------------- 
void HarmonicDialog::CountFunctionValues()
{
	m_a0_func_value = m_a0_value;
	m_f0_func_value = m_f0_value*5;
}

// ---------------  --------------- 
void HarmonicDialog::SetStaticData()
{
	wchar_t str[255] = {0};
	m_a0_stvalue.SetWindowText( _itow(m_a0_func_value, str, 10) ) ;
	m_f0_stvalue.SetWindowText( _itow(m_f0_func_value, str, 10) ) ;
}

void HarmonicDialog::RecountHarmonics()
{
	// recount functions without correlation
	ValuesSet vals;
	HarmonicsComposition::Compose(_harmonics, vals);
	_master->RecountSignal(vals);
}

void HarmonicDialog::UpdateControls()
{
	UpdateData();
	CountFunctionValues();
	SetStaticData();
	SetLine( GetSelectedLine() );
}

// ---------------  --------------- 
ListHarmonicData* HarmonicDialog::AddListItemData(int nItem, int a0, int f0)
{
	ListHarmonicData* newPeriodic = new ListHarmonicData(a0,f0);
	DWORD_PTR itemData = reinterpret_cast<DWORD_PTR>(newPeriodic);
	m_funcList.SetItemData(nItem, itemData);
	_harmonics.push_back(newPeriodic);
	return newPeriodic;
}

// ---------------  --------------- 
void HarmonicDialog::DeleteListItemData(int nItem)
{
	DWORD_PTR itemData = m_funcList.GetItemData(nItem);
	ListHarmonicData* Periodic = reinterpret_cast<ListHarmonicData*>(itemData);
	_harmonics.remove(Periodic);
	delete Periodic;
}


// ---------------  --------------- 
void HarmonicDialog::CleanListItemData()
{
	for(int i= 0 ; i < m_funcList.GetItemCount() ; i++ )
		delete ( reinterpret_cast<ListHarmonicData*>( m_funcList.GetItemData(i) ) );
}

// ---------------  --------------- 
void HarmonicDialog::OnLvnItemchangedFuncList(NMHDR *pNMHDR, LRESULT *pResult)
{
	int item = -1;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// Если элемент выделяется
	if(pNMLV->uNewState == 3)
	{
		// здесь синхронизировать значение со слайдером
		item = pNMLV->iItem;
		DWORD_PTR itemData = m_funcList.GetItemData(item);
		ListHarmonicData* Periodic = reinterpret_cast<ListHarmonicData*>(itemData);

		m_a0_slider.SetPos(Periodic->a0);
		m_f0_slider.SetPos(Periodic->f0);
		UpdateData();
	}

	if ( GetSelectedLine() != -1 )
		EnableSliders(TRUE);
	else
	{
		SetSlidersPos(0, 0);
		EnableSliders(FALSE);
	}
		
	*pResult = 0;
}
