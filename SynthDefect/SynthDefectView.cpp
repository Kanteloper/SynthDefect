
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
#include "Background.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSynthDefectView

IMPLEMENT_DYNCREATE(CSynthDefectView, CView)

BEGIN_MESSAGE_MAP(CSynthDefectView, CView)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CSynthDefectView construction/destruction

CSynthDefectView::CSynthDefectView() noexcept
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

	if (m_bInitGL) 
		InitChildView();											//Initialization of the GL window if first call
	DrawGLScene();															
}


/// <summary>
/// Render the whole OpenGL scene
/// </summary>
/// <returns> TRUE if rendering is successful, FALSE on failure </returns>
BOOL CSynthDefectView::DrawGLScene()
{
	// clear buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);

	DrawBackground();
	if (m_model)
		DrawLoadedModel();
	return TRUE;
}


/// <summary>
/// Render the default background mesh
/// </summary>
void CSynthDefectView::DrawBackground()
{
	glDisable(GL_DEPTH_TEST);										// disables Depth Testing
	glPolygonMode(GL_FRONT, GL_FILL);
	m_backgroundShader.Use();
	CBackground back = CBackground(glm::vec3(1.0f , 1.0f, 0.0f));
	back.Draw();
}


/// <summary>
/// Render the loaded model mesh from Document
/// </summary>
void CSynthDefectView::DrawLoadedModel()
{
	// reset camera
	glEnable(GL_DEPTH_TEST);										// enables Depth Testing
	// The Type Of Depth Testing To Do
	// GL_LEQUAL : Passes if the incoming depth value is less than or equal to the stored depth value.
	glDepthFunc(GL_LEQUAL);
	SetWireFrameMode(m_bWireframe);	
	// enable shaders
	m_modelShader.Use();

	// view, projection transformations
	// aspect - the ratio of width to height
	// note that the aspect ratio in glm::perspective should match the aspect ratio of the Viewport
	m_projMatrix = glm::perspective(glm::radians(m_camera->m_Zoom), m_viewWidth / m_viewHeight, 0.1f, 100.0f);
	m_viewMatrix = m_camera->GetViewMatrix();
	m_modelShader.SetMat4("projection", m_projMatrix);
	m_modelShader.SetMat4("view", m_viewMatrix);

	// model transformation: Scale -> Rotation -> Translation
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	// Scale the model
	modelMatrix = glm::scale(modelMatrix, glm::vec3(m_scaleFactor, m_scaleFactor, m_scaleFactor));			// control the scale of the model
	
	// Rotate the model using Euler's Angles
	// Rotation order is Y-axis => X-axis => Z-axis for minimize Gimble Lock
	modelMatrix = glm::rotate(modelMatrix, m_angleY, glm::vec3(0.0f, 1.0f, 0.0f));							// Y-axis rotation
	modelMatrix = glm::rotate(modelMatrix, m_angleX, glm::vec3(1.0f, 0.0f, 0.0f));							// X-axis rotation

	// Translate the model
	modelMatrix = glm::translate(modelMatrix, -m_modelCenter);												// translate to the origin
	m_modelShader.SetMat4("model", modelMatrix);

	// for lightning
	glm::vec3 lightColor = glm::vec3(LIGHT_R, LIGHT_G, LIGHT_B);
	m_modelShader.SetVec3("lightColor", lightColor);
	glm::vec3 lightPosition = glm::vec3(LIGHT_X, LIGHT_Y, LIGHT_Z);
	m_modelShader.SetVec3("lightPos", lightPosition);
	m_modelShader.SetVec3("viewPos", m_camera->GetPosition());

	m_model->DrawModel(m_modelShader);
}


void CSynthDefectView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)		
{
	CSynthDefectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	m_model = pDoc->m_model;															// receive data from Document
	if (m_model)																		// check the model is loaded
	{
		m_modelCenter = GetModelCentroid(m_model->m_max, m_model->m_min);
		InitSettings();
		m_scaleFactor = GetScaleFactor(m_model->m_max, m_model->m_min, m_modelCenter);	// calculate scale factor
	}
	else
		delete m_camera;
}


/// <summary>
/// Reset the settings of Camera
/// </summary>
void CSynthDefectView::InitSettings()
{
	// Camera
	m_cameraPos = glm::vec3(0.0f, 0.0f, 40.0f);
	m_angleX = 0.0f;
	m_angleY = 0.0f;
	m_camera = new CCamera(m_cameraPos, m_modelCenter);
	m_camera->m_Zoom = 45.0f;

	// model
	m_bWireframe = FALSE;
}


/// <summary>
/// Calculate the geometry centroid of loaded model mesh
/// </summary>
/// <param name="max">: the max value of x, y, z </param>
/// <param name="min">: the min value of x, y, z </param>
/// <returns> the geometry centroid </returns>
glm::vec3 CSynthDefectView::GetModelCentroid(glm::vec3 max, glm::vec3 min)
{
	float centerX = (max.x + min.x) / 2.0f;
	float centerY = (max.y + min.y) / 2.0f;
	float centerZ = (max.z + min.z) / 2.0f;
	return glm::vec3(centerX, centerY, centerZ);
}


/// <summary>
/// Calculate the scale factor for fitting model into viewport
/// </summary>
/// <param name="max">: the max value of x, y, z </param>
/// <param name="min">: the min value of x, y, z </param>
/// <returns> the scale factor for loaded model </returns>
float CSynthDefectView::GetScaleFactor(glm::vec3 max, glm::vec3 min, glm::vec3 center)
{
	float r = glm::distance(center, max);									// the radius of bounding sphere
	float z = glm::distance(center, m_cameraPos);							// the distance from model to camera
	float r_max = z * glm::sin(glm::radians(m_camera->GetFOV())/2.0f);		// the maximum radius of bounding sphere
	return r_max / r;
}



void CSynthDefectView::InitChildView()
{
	m_bInitGL = FALSE;
	m_backgroundShader = CShader(VS_BACKGROUND_PATH, FS_BACKGROUND_PATH);		// build and compile shaders for backgroun mesh
	m_modelShader = CShader(VS_MODEL_PATH, FS_MODEL_PATH);						// build and compile shaders for model mesh
}



CSynthDefectView::~CSynthDefectView()
{
	delete m_camera;
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

	// create a opengl child view to occupy the client area of the frame
	CreateGLWindow(this, 32);
	InitializeGLEngine();

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
	m_viewWidth = (float)cx;
	m_viewHeight = (float)cy;
}


// Render loop
void CSynthDefectView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case ID_TIMER_PLAY:
		if (DrawGLScene())
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


BOOL CSynthDefectView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (nFlags != MK_MBUTTON) 
	{
		if (m_camera)
		{
			if (zDelta < 0)
			{
				if (m_camera->m_Zoom >= MAX_ZOOM)
					m_camera->m_Zoom = MAX_ZOOM;
				else
					m_camera->m_Zoom += ZOOM_OFFSET;
			}
			else
			{
				if (m_camera->m_Zoom <= MIN_ZOOM)
					m_camera->m_Zoom = MIN_ZOOM;
				else
					m_camera->m_Zoom -= ZOOM_OFFSET;
			}
		}
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CSynthDefectView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_camera)
	{
		if (nFlags == MK_RBUTTON)
		{
			float laterX = (float)point.x;
			float laterY = (float)point.y;
			float deltaX = (laterX - m_currentX);
			float deltaY = (m_currentY - laterY);				// reversed since y-coordinates go from bottom to top

			if (deltaY > m_camera->GetSensitivity())
				m_angleX -= ROTATION_OFFSET;
			else if (deltaY < -m_camera->GetSensitivity())
				m_angleX += ROTATION_OFFSET;
			m_currentY = laterY;

			if (deltaX > m_camera->GetSensitivity())
				m_angleY += ROTATION_OFFSET;
			else if (deltaX < -m_camera->GetSensitivity())
				m_angleY -= ROTATION_OFFSET;
			m_currentX = laterX;
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}


void CSynthDefectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_camera)
	{
		glm::vec3 forward = m_camera->GetForwardAxis();
		glm::vec3 left = m_camera->GetLeftAxis();
		glm::vec3 up = m_camera->GetUpAxis();

		// viewport coordinates
		TRACE2("Log: viweport x = %d, screen y = %d\n", point.x, point.y);

		// NDC coordinates
		float x = (2.0f * (float)point.x) / m_viewWidth - 1.0f;
		float y = 1.0f - (2.0f * (float)point.y) / m_viewHeight;
		float z = -1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		TRACE3("Log: ndc x = %f, y = %f, z = %f\n", ray_nds.x, ray_nds.y, ray_nds.z);

		// Clip coordinates
		glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0f);
		TRACE3("Log: clip x = %f, y = %f, z = %f\n", ray_clip.x, ray_clip.y, ray_clip.z);

		// Eye coordinates
		glm::vec4 ray_eye = glm::inverse(m_projMatrix) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 1.0);			// unproject only x, y part
		TRACE3("Log: eye x = %f, y = %f, z = %f\n", ray_eye.x, ray_eye.y, ray_eye.z);

		// 4d World coordinates
		glm::vec3 ray_world = glm::vec3(glm::inverse(m_viewMatrix) * ray_eye);
		ray_world = glm::normalize(ray_world);
		TRACE3("Log: world x = %f, y = %f, z = %f\n", ray_world.x, ray_world.y, ray_world.z);







	}
	CView::OnLButtonDown(nFlags, point);
}


void CSynthDefectView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_camera)
	{
		m_currentX = (float)point.x;
		m_currentY = (float)point.y;
	}
	CView::OnRButtonDown(nFlags, point);
}


void CSynthDefectView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_model)
	{
		// press Tab key
		if (nFlags == 15)
		{
			if (m_bWireframe)
				m_bWireframe = FALSE;
			else
				m_bWireframe = TRUE;
		}
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
