
// CGLWnd.h : interface of the CGLWnd class

#pragma once
#include <afxwin.h>


class CGLWnd : public CWnd
{
// Construction
public:
	CGLWnd();

// Attributes
public:
	BOOL m_bInitGL;

// Operations
public:

// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CGLWnd();



};

