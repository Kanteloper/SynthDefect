
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
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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
	m_projMatrix = glm::perspective(glm::radians(m_camera->GetZoom()), m_viewWidth / m_viewHeight, 0.1f, 100.0f);
	m_viewMatrix = m_camera->GetViewMatrix();
	// Rotate the model using Euler's Angles
	// Rotation order is Y-axis => X-axis => Z-axis for minimize Gimble Lock
	m_viewMatrix = glm::rotate(m_viewMatrix, m_angleY, glm::vec3(0.0f, 1.0f, 0.0f));							// Y-axis rotation
	m_viewMatrix = glm::rotate(m_viewMatrix, m_angleX, glm::vec3(1.0f, 0.0f, 0.0f));							// X-axis rotation
	m_modelShader.SetMat4("projection", m_projMatrix);
	m_modelShader.SetMat4("view", m_viewMatrix);

	// model transformation: Scale -> Rotation -> Translation
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scaleFactor, m_scaleFactor, m_scaleFactor));			// control the scale of the model
	m_modelMatrix = glm::translate(m_modelMatrix, -m_modelCenter);												// translate to the origin
	m_modelShader.SetMat4("model", m_modelMatrix);

	// lightning
	glm::vec3 lightColor = glm::vec3(LIGHT_R, LIGHT_G, LIGHT_B);
	m_modelShader.SetVec3("lightColor", lightColor);
	glm::mat4 m_lightMatrix = glm::mat4(1.0f);
	m_lightMatrix = glm::rotate(m_lightMatrix, -m_angleX, glm::vec3(1.0f, 0.0f, 0.0f));							// X-axis rotation
	m_lightMatrix = glm::rotate(m_lightMatrix, -m_angleY, glm::vec3(0.0f, 1.0f, 0.0f));							// Y-axis rotation
	glm::vec3 lightPosition = glm::vec3(m_lightMatrix * glm::vec4(LIGHT_X, LIGHT_Y, LIGHT_Z, 1.0));
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
	m_cameraPos = glm::vec3(0.0f, 0.0f, DISTANCE_FROM_MODEL);
	m_angleX = 0.0f;
	m_angleY = 0.0f;
	m_camera = new CCamera(m_cameraPos, m_modelCenter);
	m_camera->SetZoom(45.0f);

	// Flag
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
	float r_max = z * glm::sin(glm::radians(FOV)/2.0f);		// the maximum radius of bounding sphere
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
				if (m_camera->GetZoom() >= MAX_ZOOM)
					m_camera->SetZoom(MAX_ZOOM);
				else
					m_camera->SetZoom(m_camera->GetZoom() + ZOOM_OFFSET);
			}
			else
			{
				if (m_camera->GetZoom() <= MIN_ZOOM)
					m_camera->SetZoom(MIN_ZOOM);
				else
					m_camera->SetZoom(m_camera->GetZoom() - ZOOM_OFFSET);
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

			if (deltaY > CAMERA_SENSITIVITY)
				m_angleX -= ROTATION_OFFSET;
			else if (deltaY < -CAMERA_SENSITIVITY)
				m_angleX += ROTATION_OFFSET;
			m_currentY = laterY;

			if (deltaX > CAMERA_SENSITIVITY)
				m_angleY += ROTATION_OFFSET;
			else if (deltaX < -CAMERA_SENSITIVITY)
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
		glm::vec3 pickedPoint = CalculateMouseRay(point);
		TRACE3("Log: pickedPoint x = %f, y = %f, z = %f\n", pickedPoint.x, pickedPoint.y, pickedPoint.z);

		
		// Normals Test
		if (m_model)
		{
			std::vector<aiFace> faces = m_model->GetFacesFromModel();
			// iterate all faces(triangles)
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				aiFace face = faces[i];
				// iterate all vertices in face
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					std::vector<Vertex> vertices = m_model->GetVerticesFromModel();
					unsigned int index = face.mIndices[j];
					glm::vec3 test = m_modelMatrix * glm::vec4(vertices[index].Position, 1.0);
					test = glm::normalize(test);
					//TRACE3("Log: world2 x = %f, y = %f, z = %f\n", test.x, test.y, test.z);
					// get three points of a face
					// check whether the ray point is in the face

				}
			}
		}
			
	}
	CView::OnLButtonDown(nFlags, point);
}


/// <summary>
/// Calculate ray picking point which is the position of mouse clicked
/// </summary>
/// <returns> the world coordinates of the position ouf mouse clicked</returns>
glm::vec3 CSynthDefectView::CalculateMouseRay(CPoint p)
{
	glm::vec2 normalizedCoords = GetNormalizedDeviceCoords(p);
	glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	TRACE3("Log: eyeCoords x = %f, y = %f, z = %f\n", eyeCoords.x, eyeCoords.y, eyeCoords.z);
	glm::vec3 worldRay = toWorldCoords(eyeCoords);
	TRACE3("Log: worldRay x = %f, y = %f, z = %f\n", worldRay.x, worldRay.y, worldRay.z);
	return glm::normalize(worldRay);
}


/// <summary>
/// Calculate the view port coordinates to Normalized Device Coordinates
/// </summary>
/// <param name="p">: the view port coordinates </param>
/// <returns> the Normalized Device Coordinates </returns>
glm::vec2 CSynthDefectView::GetNormalizedDeviceCoords(CPoint p)
{
	float x = 2.0f * (float)p.x / m_viewWidth - 1.0f;
	float y = 1.0f - (2.0f * (float)p.y) / m_viewHeight;
	return glm::vec2(x, y);
}


/// <summary>
/// Calculate the Clip coordinates to Eye coordinates
/// </summary>
/// <param name="clip">: the clip coordinates </param>
/// <returns> the eye coordinates </returns>
glm::vec4 CSynthDefectView::toEyeCoords(glm::vec4 clip)
{
	glm::vec4 eye = glm::inverse(m_projMatrix) * clip;
	return glm::vec4(eye.x, eye.y, -1.0f, 1.0f);
}


/// <summary>
/// Calculate the Eye coordinates to the World coordinates
/// </summary>
/// <param name="eye">: the eye coordinates </param>
/// <returns> the world coordinates </returns>
glm::vec3 CSynthDefectView::toWorldCoords(glm::vec4 eye)
{
	glm::vec4 world = glm::inverse(m_viewMatrix) * eye;
	return glm::vec3(world.x, world.y, world.z);
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
