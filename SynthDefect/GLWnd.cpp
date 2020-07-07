
// GLWnd.cpp : implementation of the CGLWnd class


#include "pch.h"
#include "GLWnd.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#ifdef DEBUG
#define new DEBUG_NEW						// Assists in finding memory leaks in debug mode				
#endif

BEGIN_MESSAGE_MAP(CGLWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

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
	// WS_BORDER: a window has a border
	cs.style &= ~WS_BORDER; // &=  operator :  remove WS_BORDER style
	// CS_HREDRAW: Redraws the entire window if a movement or size adjustment changes the width of the client area.
	// CS_VREDRAW: Redraws the entire window if a movement or size adjustment changes the height of the client area.
	// CS_DBLCLKS: Sends a double-click message to the window procedure when the user double-clicks the mouse
	//             while the cursor is within a window belonging to the class.
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL); // allow to register own window classes

	return TRUE;
}


// Call when Windows or an application makes a request to repaint a portion of an application's window
// Use when update or redraw window
void CGLWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages

	if (m_bInitGL) InitGL(); //Initialization of the GL window if first call
	// DrawGLSceneN();					//Global drawing procedure
}

// All Setup for OpenGL
void CGLWnd::InitGL()
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing

	// The Type Of Depth Testing To Do
	// GL_LEQUAL : Passes if the incoming depth value is less than or equal to the stored depth value.
	glDepthFunc(GL_LEQUAL);								

	// Specify implementation-specific hints
	// Certain aspects of GL behavior can be controlled with hints.
	// GL_PERSPECTIVE_CORRECTION_HINT: Indicates the quality of color, texture coordinate, and fog(안개) coordinate interpolation.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

