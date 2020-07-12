
// SynthDefectView.cpp : implementation of the CSynthDefectView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SynthDefect.h"
#endif

#include "SynthDefectDoc.h"
#include "SynthDefectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER_PLAY 100					// Unique ID of system timer for a specific window


// CSynthDefectView

IMPLEMENT_DYNCREATE(CSynthDefectView, CView)

BEGIN_MESSAGE_MAP(CSynthDefectView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSynthDefectView construction/destruction

CSynthDefectView::CSynthDefectView() noexcept
	: m_bInitGL(TRUE)
{
	// TODO: add construction code here
}

CSynthDefectView::~CSynthDefectView()
{
}

// 생성되는 window의 다양한 속성 변경
BOOL CSynthDefectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// WS_EX_CLIENTEDGE: Specify that a window has a 3D look - a border with a sunken edge
	// WS_BORDER: a window has a border
	cs.style &= ~WS_BORDER; // &=  operator :  remove WS_BORDER style
	// CS_HREDRAW: Redraws the entire window if a movement or size adjustment changes the width of the client area.
	// CS_VREDRAW: Redraws the entire window if a movement or size adjustment changes the height of the client area.
	// CS_DBLCLKS: Sends a double-click message to the window procedure when the user double-clicks the mouse
	//             while the cursor is within a window belonging to the class.
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL); // allow to register own window classes

	return CView::PreCreateWindow(cs);
}


// CSynthDefectView drawing
void CSynthDefectView::OnDraw(CDC* /*pDC*/)
{
	CSynthDefectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (m_bInitGL) InitChildView();											//Initialization of the GL window if first call
	DrawGLScene();															//Global drawing procedure
}


/// <summary>
/// 
/// </summary>
void CSynthDefectView::InitChildView()
{
	m_bInitGL = FALSE;
	InitGL();
}


void CSynthDefectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSynthDefectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSynthDefectView diagnostics

#ifdef _DEBUG
void CSynthDefectView::AssertValid() const
{
	CView::AssertValid();
}

void CSynthDefectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSynthDefectDoc* CSynthDefectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSynthDefectDoc)));
	return (CSynthDefectDoc*)m_pDocument;
}
#endif //_DEBUG


// CSynthDefectView message handlers


BOOL CSynthDefectView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


BOOL CSynthDefectView::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::DestroyWindow();
}


void CSynthDefectView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


void CSynthDefectView::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CView::PostNcDestroy();
}


int CSynthDefectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	// create a opengl child view to occupy the client area of the frame
	CreateGLWindow(this, 32);
	// SetTimer - Install a system timer
	SetTimer(ID_TIMER_PLAY, 15, NULL);
	// m_wndChild.Create(TEXT("STATIC"), TEXT("DEMO"), WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(30, 30, 180, 180), this, 1234);
	return 0;
}


void CSynthDefectView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	bool bRslt = FALSE;
	switch (nIDEvent)
	{
	case ID_TIMER_PLAY:
		bRslt = DrawGLScene();									// OpenGL drawing procedure
		if (bRslt)
			SwapBuffers(wglGetCurrentDC());
		break;
	}
	CView::OnTimer(nIDEvent);
}


void CSynthDefectView::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CView::OnClose();
}


void CSynthDefectView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
}


void CSynthDefectView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}