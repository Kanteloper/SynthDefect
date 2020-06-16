
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


// CSynthDefectView

IMPLEMENT_DYNCREATE(CSynthDefectView, CView)

BEGIN_MESSAGE_MAP(CSynthDefectView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CSynthDefectView construction/destruction

CSynthDefectView::CSynthDefectView() noexcept
{
	// TODO: add construction code here
}

CSynthDefectView::~CSynthDefectView()
{
}

BOOL CSynthDefectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSynthDefectView drawing

void CSynthDefectView::OnDraw(CDC* /*pDC*/)
{
	CSynthDefectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
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
	// AFX_IDW_PANE_FIRST :  
	if (!m_glwndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create opengl view window\n");
		return -1;
	}
	return 0;
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
