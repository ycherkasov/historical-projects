// ChartDemo2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CChartDemo2App:
// See ChartDemo2.cpp for the implementation of this class
//

class CChartDemo2App : public CWinApp
{
public:
	CChartDemo2App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CChartDemo2App theApp;