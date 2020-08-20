
// SynthDefect.h : main header file for the SynthDefect application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


// CSynthDefectApp:
// See SynthDefect.cpp for the implementation of this class
//

class CSynthDefectApp : public CWinAppEx
{
public:
	CSynthDefectApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual int Run();
};

extern CSynthDefectApp theApp;
