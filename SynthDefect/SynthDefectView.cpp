
// SynthDefectView.cpp : implementation of the CSynthDefectView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SynthDefect.h"
#endif

#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/GLU.h>
#include "SynthDefectDoc.h"
#include "SynthDefectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER_PLAY 100							// Unique ID of system timer for a specific window
#define VSHADER_CODE_PATH "./vertex_shader.glsl"		// The current path of vertex shader code file
#define FSHADER_CODE_PATH "./fragment_shader.glsl"	// The current path of fragment shader code file


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
	ON_WM_SIZE()
//  ON_WM_UPDATEUISTATE()
END_MESSAGE_MAP()

// CSynthDefectView construction/destruction

CSynthDefectView::CSynthDefectView() noexcept
	: m_bInitGL(TRUE)
{
	// TODO: add construction code here
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

	if (m_bInitGL) 
		InitChildView();											//Initialization of the GL window if first call

	if (m_model)
		DrawGLScene();															
}


/// <summary>
/// Initialize global setting for rendering using OpenGL
/// </summary>
void CSynthDefectView::InitGL()
{
	// Initialize Camera
	m_camera = CCamera(glm::vec3(3.0f, 0.0f, 0.0f));
	// Initialize glew entry points to create a valid OpenGL rendering context
	glewExperimental = TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		// Problem: glewInit failed, something is seriously wrong.
		TRACE("Log: failed to initialize glew");
		exit(1);
	}
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	// The Type Of Depth Testing To Do
	// GL_LEQUAL : Passes if the incoming depth value is less than or equal to the stored depth value.
	glDepthFunc(GL_LEQUAL);
}


/// <summary>
/// Render the OpenGL scene
/// </summary>
/// <returns> TRUE if rendering is successful, FALSE on failure </returns>
int CSynthDefectView::DrawGLScene()
{
	// clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
	// set background
	// SetGLBackground();
	// enable shaders
	m_shaders.Use();

	// view/projection transformations
	CRect rect;
	this->GetClientRect(&rect);
	glm::mat4 projMatrix = glm::perspective(glm::radians(m_camera.Zoom), (float)rect.Width() / (float)rect.Height(), 0.1f, 100.0f);
	glm::mat4 viewMatrix = m_camera.GetViewMatrix();
	m_shaders.SetMat4("projection", projMatrix);
	m_shaders.SetMat4("view", viewMatrix);


	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	m_shaders.SetMat4("model", model);
	m_model->DrawModel(m_shaders);

	return TRUE;
}


void CSynthDefectView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CSynthDefectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// receive loaded model
	if (pDoc->m_model)
		m_model = pDoc->m_model;
}


void CSynthDefectView::InitChildView()
{
	m_bInitGL = FALSE;
	// configure global opengl state
	InitGL();
	// build and compile shaders
	m_shaders = CShader(VSHADER_CODE_PATH, FSHADER_CODE_PATH);
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


CSynthDefectView::~CSynthDefectView()
{
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
	SetTimer(ID_TIMER_PLAY, 10, NULL);
	
	return 0;
}

/// <summary>
/// message handler that calls after the window's size has changed
/// </summary>
/// <param name="nType">: Specify the type of resizing requested</param>
/// <param name="cx">: Specify the new width of the client area</param>
/// <param name="cy">: Specify the new height of the client area</param>
void CSynthDefectView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	ResizeGLScene(cx, cy);
}

// Render loop
void CSynthDefectView::OnTimer(UINT_PTR nIDEvent)
{
	BOOL bRslt = FALSE;
	switch (nIDEvent)
	{
	case ID_TIMER_PLAY:
		if (m_model)
			bRslt = DrawGLScene();
		else
			break;
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