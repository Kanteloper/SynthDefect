
// GLWnd.cpp : implementation of the CGLWnd class


#include "pch.h"
#include "GLWnd.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#ifdef DEBUG
#define new DEBUG_NEW						// Assists in finding memory leaks in debug mode				
#endif

CGLWnd::CGLWnd():m_bInitGL(TRUE)
{
}

CGLWnd::~CGLWnd()
{
}

// CGLWnd message handlers
// 생성되는 window의 다양한 속성 변경
BOOL CGLWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	
	// WS_EX_CLIENTEDGE: Specify that a window has a 3D look - a border with a sunken edge
	cs.dwExStyle |= WS_EX_CLIENTEDGE; // |= - add a new style
	// WS_BORDER: a window has a border
	cs.style &= ~WS_BORDER; // &= - remove WS_BORDER style
	// CS_HREDRAW: Redraws the entire window if a movement or size adjustment changes the width of the client area.
	// CS_VREDRAW: Redraws the entire window if a movement or size adjustment changes the height of the client area.
	// CS_DBLCLKS: Sends a double-click message to the window procedure when the user double-clicks the mouse while the cursor is within a window belonging to the class.
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL); // allow to register own window classes

	return TRUE;
}


