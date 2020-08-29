
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
#include <glm/gtx/string_cast.hpp>
#include <iostream>
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


BOOL CSynthDefectView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
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


void CSynthDefectView::InitChildView()
{
	m_bInitGL = FALSE;
	m_backgroundShader = CShader(VS_BACKGROUND_PATH, FS_BACKGROUND_PATH);		// build and compile shaders for background mesh
	m_baseShader = CShader(VS_BASE_PATH, FS_BASE_PATH);				// build and compile shaders for base plane mesh
	m_modelShader = CShader(VS_MODEL_PATH, FS_MODEL_PATH);						// build and compile shaders for model mesh
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
	DrawBasePlane();
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
/// Render the default grid plane
/// </summary>
void CSynthDefectView::DrawBasePlane()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	m_baseShader.Use();

}


/// <summary>
/// Render the loaded model mesh from Document
/// </summary>
void CSynthDefectView::DrawLoadedModel()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	SetWireFrameMode(m_bWireframe);	

	// enable shaders
	m_modelShader.Use();

	// view, projection transformations
	// note that the aspect ratio in glm::perspective should match the aspect ratio of the Viewport
	m_projMatrix = glm::perspective(glm::radians(m_camera->GetZoom()), m_viewWidth / m_viewHeight, 0.1f, 100.0f);
	m_viewMatrix = m_camera->GetViewMatrix();
	m_modelShader.SetMat4("projection", m_projMatrix);
	m_modelShader.SetMat4("view", m_viewMatrix);

	// model transformation: Scale -> Rotation -> Translation
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scaleFactor, m_scaleFactor, m_scaleFactor));			// control the scale of the model
	m_modelMatrix = glm::translate(m_modelMatrix, -m_modelCenter);												// translate to the origin
	m_modelShader.SetMat4("model", m_modelMatrix);

	// lightning
	m_modelShader.SetVec3("lightColor", m_camera->GetLightColor());
	m_modelShader.SetVec3("lightPos", m_camera->GetLightPosition());
	m_modelShader.SetVec3("cameraPos", m_camera->GetPosition());

	m_model->DrawModel(m_modelShader);
}


void CSynthDefectView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
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
		m_modelCenter = m_model->GetModelCentroid();
		InitSettings();
		m_scaleFactor = m_model->GetModelScaleFactor(m_camera->GetPosition(), m_modelCenter);	// calculate scale factor
	}
}


/// <summary>
/// Reset the settings of Camera
/// </summary>
void CSynthDefectView::InitSettings()
{
	// Camera
	m_defaultCameraPosistion = glm::vec3(CAMERA_X, CAMERA_Y, CAMERA_Z);
	m_camera = new CCamera(m_defaultCameraPosistion, m_modelCenter);
	m_camera->SetZoom(45.0f);

	// Flag
	m_bWireframe = FALSE;
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


void CSynthDefectView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}


/******************************** Mouse event handlers ********************************/

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
			m_later = GetNormalizedDeviceCoords(point);
			m_camera->Rotate(m_current, m_later);
			m_current = m_later;
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}


void CSynthDefectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_camera)
	{
		// Ray picking
		//m_rayStart = glm::vec3(glm::inverse(m_viewMatrix) * glm::vec4(0.0f, 0.0f, DISTANCE_FROM_MODEL, 1.0f));		// camera position in the world space
		glm::vec3 m_rayStart = glm::inverse(m_viewMatrix) * glm::vec4(0.0f, 0.0f, CAMERA_Z, 1.0f);		// camera position in the world space
		std::cout << "ray_start: " << glm::to_string(m_rayStart) << std::endl;
		glm::vec3 m_rayDir = CalculateMouseRay(point, m_rayStart);
		std::cout << "ray_dir: " << glm::to_string(m_rayDir) << std::endl;

		if (m_model)
		{
			m_faces = m_model->GetFacesFromModel();																	// all faces of the loaded model
			m_vertices = m_model->GetVerticesFromModel();
			// check the clicked mouse point is on the model or not using Color buffer
			// if the clicked mouse point is on the model, 
				// use Ray picking
			/*if (IsRayIntersected(m_rayStart, m_rayDir))
			{
				std::cout << "model exist" << std::endl;
			}
			else
			{
				std::cout << "model not exist" << std::endl;
			}*/
																	// all vertices of the loaded model
			
			float intersec_dis = CalIntersectedDistance(m_rayStart, m_rayDir);
			//std::cout << "intersec_dis: " << intersec_dis << std::endl;
			glm::vec3 intersec_point = m_rayStart + m_rayDir * intersec_dis;
			//std::cout << "intersec_point: " << glm::to_string(intersec_point) << std::endl;
			//ProcessNormalTest(intersec_point);

			// change the color value of the triangle which has intersected point
			// save the information of selected triangle
			
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


/// <summary>
///	
/// </summary>
/// <returns></returns>
BOOL CSynthDefectView::IsRayIntersected(glm::vec3 const& origin, glm::vec3 const& dir)
{

	for (unsigned int i = 0; i < m_faces.size(); i++)
	{
		aiFace face = m_faces[i];
		// There is three vertices in a triangle of mesh
		std::vector<glm::vec3> points = GetPointsFromFace(face);
		glm::vec3 normal = glm::normalize(glm::cross(points[1] - points[0], points[2] - points[0]));

		float t = (glm::dot(normal, origin) + glm::dot(normal, points[0])) / glm::dot(normal, dir);
		std::cout << "intersec_point: " << glm::to_string(origin + t * dir) << std::endl;
		// skip the minus value
		//if (distance < 0.0f) continue;
	}
	return FALSE;
}

	
/// <summary>
/// Calculate ray picking point which is the position of mouse clicked
/// </summary>
/// <returns> the world coordinates of the position ouf mouse clicked</returns>
glm::vec3 CSynthDefectView::CalculateMouseRay(CPoint const& p, glm::vec3 const& origin)
{
	glm::vec2 normalizedCoords = GetNormalizedDeviceCoords(p);
	glm::vec4 clip = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0, 1.0);
	glm::vec4 eye = toEyeCoords(clip);	// ray_dir_view
	glm::vec3 ray_point = toWorldCoords(eye);
	return glm::normalize(ray_point - glm::vec3(origin));
}





/// <summary>
/// Calculate the view port coordinates to Normalized Device Coordinates
/// </summary>
/// <param name="p">: the view port coordinates </param>
/// <returns> the Normalized Device Coordinates </returns>
glm::vec2 CSynthDefectView::GetNormalizedDeviceCoords(CPoint const& p)
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
glm::vec4 CSynthDefectView::toEyeCoords(glm::vec4 const& clip)
{
	glm::vec4 eye = glm::inverse(m_projMatrix) * clip;
	return glm::vec4(eye.x, eye.y, -1.0f, 1.0f);
}


/// <summary>
/// Calculate the Eye coordinates to the World coordinates
/// </summary>
/// <param name="eye">: the eye coordinates </param>
/// <returns> the world coordinates </returns>
glm::vec3 CSynthDefectView::toWorldCoords(glm::vec4 const& eye)
{
	glm::vec4 world = glm::inverse(m_viewMatrix) * eye;
	return glm::vec3(world.x, world.y, world.z);
}


/// <summary>
/// Calculate the intersected distance from the neares surface of the loaded model
/// </summary>
/// <param name="faces">: triangular surfaces of the loaded model </param>
/// <param name="vertices">: vertices of a triangular surface of the loaded model </param>
/// <returns> the distance from ray start point to the nearest surface of the loaded model </returns>
float CSynthDefectView::CalIntersectedDistance(glm::vec3 const& origin, glm::vec3 const& dir)
{
	float min_dis = MAX_DISTANCE;
	for (unsigned int i = 0; i < m_faces.size(); i++)
	{
		std::cout << "face: " << i << std::endl;
		aiFace face = m_faces[i];
		// There is three vertices in a triangle of mesh
		std::vector<glm::vec3> points = GetPointsFromFace(face);	
		float distance = CalDistanceToSurface(points, origin, dir);
		// skip the minus value
		//if (distance < 0.0f) continue;
		std::cout << "distance: " << distance << std::endl;
		// find the nearest surface
		/*if (min_dis >= distance)
		{
			min_dis = distance;
			std::cout << "selected face: " << i << std::endl;
			SaveFaceInfo(i);
		}*/
	}
	return min_dis;
}


/// <summary>
/// Save the index of triangular faces which is at the nearest distance from ray start point
/// </summary>
/// <param name="index">: the index for retrieving the faces </param>
void CSynthDefectView::SaveFaceInfo(int const& index)
{
	if (m_faceQueue.size() == 2)
		m_faceQueue.pop();
	m_faceQueue.push(index);
}


/// <summary>
/// Calculate the distance from ray_start point to the nearest surface of the loaded model
/// </summary>
/// <param name="points">: three points of a triangle </param>
/// <returns> the intersected distance </returns>
float CSynthDefectView::CalDistanceToSurface(std::vector<glm::vec3> const& points, glm::vec3 const& origin, glm::vec3 const& dir)
{
	// change local coordinates to world coordinates
	glm::vec3 A = m_modelMatrix * glm::vec4(points[0], 1.0);
	glm::vec3 B = m_modelMatrix * glm::vec4(points[1], 1.0);
	glm::vec3 C = m_modelMatrix * glm::vec4(points[2], 1.0);
	glm::vec3 normal = glm::normalize(glm::cross(B - A, C - A));
	return (glm::dot(origin, normal) + 50.0f / glm::dot(dir, normal));
}


/// <summary>
/// Check the intersected point is inside or on the triangle using surface normal
/// </summary>
/// <param name="faces">all triangular faces of the loaded model </param>
/// <param name="vertices">: all vectices of the loaded model </param>
/// <param name="p">: the intersected point </param>
void CSynthDefectView::ProcessNormalTest(glm::vec3 const& p)
{
	while (!m_faceQueue.empty())
	{
		aiFace face = m_faces[m_faceQueue.front()];
		std::cout << "face: " << m_faceQueue.front() << std::endl;
		m_faceQueue.pop();
		std::vector<glm::vec3> points = GetPointsFromFace(face);
		for (int i = 0; i < points.size(); i++)
		{
			std::cout << "point: " << glm::to_string(points[i]) << std::endl;
		}
		/*if (IsPointInOrOnTriangle(p))
		{

		}
		else
		{

		}*/
	}
}


/// <summary>
/// Retrieve three points from the triangular face in Assip tree
/// </summary>
/// <param name="f">: the face we want to retrieve its points </param>
/// <returns> Three XYZ coordinates of the face </returns>
std::vector<glm::vec3> CSynthDefectView::GetPointsFromFace(aiFace const& f)
{
	std::vector<glm::vec3> p;
	for (unsigned int i = 0; i < f.mNumIndices; i++)
	{
		unsigned int index = f.mIndices[i];
		//std::cout << "point: " << glm::to_string(m_vertices[index].Position) << std::endl;
		p.push_back(m_vertices[index].Position);
	}
	return p;
}


void CSynthDefectView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_camera)
	{
		m_current = GetNormalizedDeviceCoords(point);
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


CSynthDefectView::~CSynthDefectView()
{
	if (m_camera)
		delete m_camera;
}

/**************************************************************************************/

void CSynthDefectView::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CView::OnClose();
}


BOOL CSynthDefectView::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::DestroyWindow();
}


void CSynthDefectView::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CView::PostNcDestroy();
}


void CSynthDefectView::OnDestroy()
{
	CView::OnDestroy();
}
