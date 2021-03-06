
// SynthDefectView.cpp : implementation of the CSynthDefectView class

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
#include <random>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Exporter.hpp"

#include "SynthDefectDoc.h"
#include "SynthDefectView.h"
#include "Pipeline.h"

#include <array>

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
	ON_MESSAGE(UM_EXECUTE_PIPELINE, &CSynthDefectView::OnExecutePipeline)
END_MESSAGE_MAP()

// CSynthDefectView construction/destruction

CSynthDefectView::CSynthDefectView() noexcept
{
	m_viewMatrix = glm::mat4();
	m_projMatrix = glm::mat4();
	m_modelMatrix = glm::mat4();
	m_defaultCameraPosistion = glm::vec3();
	m_current = glm::vec2();
	m_later = glm::vec2();
	m_viewWidth = 0.0f;
	m_viewHeight = 0.0f;
	m_angleX = 0.0f;
	m_angleY = 0.0f;
	m_modelScale = 1.0f;
	m_modelCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	m_hMainFrm = NULL;
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

	m_hMainFrm = lpCreateStruct->hwndParent;

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
	if (m_bInitGL) 
		InitChildView();											//Initialization of the GL window if first call
	DrawGLScene();															
}

void CSynthDefectView::InitChildView()
{
	m_bInitGL = FALSE;
	m_back = new CBackground();													// init background mesh
	m_backgroundShader = CShader(VS_BACKGROUND_PATH, FS_BACKGROUND_PATH);		// build and compile shaders for background mesh
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
	m_back->Draw();
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
	m_projMatrix = glm::perspective(glm::radians(m_camera->GetZoom()), m_viewWidth / m_viewHeight, NEAR_PLANE, FAR_PLANE);
	m_viewMatrix = m_camera->GetViewMatrix();
	m_modelShader.SetMat4("projection", m_projMatrix);
	m_modelShader.SetMat4("view", m_viewMatrix);

	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_modelScale, m_modelScale, m_modelScale));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_angleY, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_angleX, glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::translate(m_modelMatrix, -m_modelCenter);
	m_modelShader.SetMat4("model", m_modelMatrix);

	// lightning
	m_modelShader.SetVec3("lightColor", m_camera->GetLightColor());
	m_modelShader.SetVec3("lightPos", m_camera->GetLightPosition());
	m_modelShader.SetVec3("cameraPos", m_camera->GetPosition());

	m_model->DrawModel();
}

void CSynthDefectView::OnInitialUpdate()
{
	CSynthDefectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_model = pDoc->m_model;	// receive data from Document
	m_base = pDoc->m_base;		// initialize the base mesh for synthetic defect
	CView::OnInitialUpdate();
}

void CSynthDefectView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)		
{
	if (m_model)
	{
		m_modelCenter = m_model->GetModelCentroid();
		InitSettings();
		m_modelScale = m_model->GetModelScaleFactor(m_camera->GetPosition(), m_modelCenter);	// calculate scale factor
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

	// Model
	m_angleX = m_angleY = 0.0f;

	// Flag
	m_bWireframe = FALSE;

	// Picking
	m_pickedFaces.clear();
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

afx_msg LRESULT CSynthDefectView::OnExecutePipeline(WPARAM wParam, LPARAM lParam)
{

	Properties* properties = reinterpret_cast<Properties*>(wParam);
	if (m_model)
	{
		if (!m_pickedFaces.empty())
		{
			CPipeline pip(*properties, m_pickedFaces, m_model, m_base);
			pip.Execute();
		}
		else
		{
			AfxMessageBox(_T("You should select faces of model to locate synthetic defects."));
			PostMessageA(m_hMainFrm, UM_GENERATE_ERROR, 0, 0);
		}
	}
	else
	{
		AfxMessageBox(_T("There are no model to execute pipelines."));
		PostMessageA(m_hMainFrm, UM_GENERATE_ERROR, 0, 0);
	}
	return 0;
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
				if (m_camera->GetZoom() >= MIN_ZOOM)
					m_camera->SetZoom(MIN_ZOOM);
				else
					m_camera->SetZoom(m_camera->GetZoom() + ZOOM_OFFSET);
			}
			else
			{
				if (m_camera->GetZoom() <= MAX_ZOOM)
					m_camera->SetZoom(MAX_ZOOM);
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
			glm::vec2 later = glm::vec2(point.x, point.y);
			float deltaX = (later.x - m_current.x);
			float deltaY = (m_current.y - later.y);				// reversed since y-coordinates go from bottom to top

			if (deltaY > SENSITIVITY)
				m_angleX -= ROTATION_OFFSET;
			else if (deltaY < -SENSITIVITY)
				m_angleX += ROTATION_OFFSET;
			m_current.y = later.y;

			if (deltaX > SENSITIVITY)
				m_angleY += ROTATION_OFFSET;
			else if (deltaX < -SENSITIVITY)
				m_angleY -= ROTATION_OFFSET;
			m_current.x = later.x;
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}

void CSynthDefectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_camera && m_model)
	{
		// ray casting
		glm::vec3 m_rayStart = toWorldCoords(glm::vec4(m_defaultCameraPosistion, 1.0f));
		glm::vec3 m_rayDir = CalculateMouseRay(point, m_rayStart);
		m_faces = m_model->GetFacesFromModel();
		m_vertices = m_model->GetVerticesFromModel();
		int indexOfpickedFace = GetPickedTriangle(m_rayStart, m_rayDir);

		// if there is a loaded model in the position where the user picked
		if (indexOfpickedFace != -1)
		{
			// There are some faces picked
			if (!m_pickedFaces.empty())
			{
				
				if (IsPicked(indexOfpickedFace))
				{
					// If the face is already picked
					int offset = FindDuplicatedFace(indexOfpickedFace);
					m_pickedFaces.erase(m_pickedFaces.begin() + offset);
					ChangePickedFaceColor(indexOfpickedFace, glm::vec4(MODEL_R, MODEL_G, MODEL_B, MODEL_A));
				}
				else
				{
					// If the face is picked at first
					m_pickedFaces.push_back(indexOfpickedFace);
					ChangePickedFaceColor(indexOfpickedFace, glm::vec4(PICK_R, PICK_G, PICK_B, PICK_A));
				}
			}
			else
			{
				// There are no faces picked
				m_pickedFaces.push_back(indexOfpickedFace);
				ChangePickedFaceColor(indexOfpickedFace, glm::vec4(PICK_R, PICK_G, PICK_B, PICK_A));
			}
		}
	}
	CView::OnLButtonDown(nFlags, point);
}

/// <summary>
///	Find the index of face(triangle) of the loaded model which is under the point that user picks
/// </summary>
/// <returns> the index of selected triangle </returns>
int CSynthDefectView::GetPickedTriangle(glm::vec3 const& origin, glm::vec3 const& dir)
{
	float minDistance = MAX_DISTANCE;
	int result = -1;

	for (unsigned int i = 0; i < m_faces.size(); i++)
	{
		aiFace face = m_faces[i];
		std::vector<glm::vec3> points = GetPointsFromFace(face);
		glm::vec3 A = m_modelMatrix * glm::vec4(points[0], 1.0f);
		glm::vec3 B = m_modelMatrix * glm::vec4(points[1], 1.0f);
		glm::vec3 C = m_modelMatrix * glm::vec4(points[2], 1.0f);
		glm::vec3 normal = glm::normalize(glm::cross(B - A, C - A));
	
		float intersectedDistance = CalculateIntersectedDistance(A, normal, origin, dir);
		if (intersectedDistance < 0.0f) continue;
		glm::vec3 intersectedPoint = CalculateIntersectedPoint(intersectedDistance, origin, dir);

		// check Normal test
		if (IsPointOnSurface(intersectedPoint, A, B, C))
		{
			// find the face of the loaded model with minimum distance
			if (intersectedDistance < minDistance)
			{
				minDistance = intersectedDistance;
				result = i;
			}
		}
	}
	return result;
}
	
/// <summary>
/// Calculate the direction of ray from picking point which is the position of mouse clicked
/// </summary>
/// <returns> the direction of the ray </returns>
glm::vec3 CSynthDefectView::CalculateMouseRay(CPoint const& p, glm::vec3 const& origin)
{
	glm::vec2 normalizedCoords = GetNormalizedDeviceCoords(p);
	glm::vec4 clip = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
	glm::vec4 eye = toEyeCoords(clip);
	glm::vec3 ray_point = toWorldCoords(glm::vec4(0.0f, 0.0f, CAMERA_Z + RAY_OFFSET, 0.0f) + eye);
	return glm::normalize(ray_point - origin);
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
/// Retrieve three points from the triangular face in Assip tree
/// </summary>
/// <param name="f">: the face we want to retrieve its points </param>
/// <returns> Three XYZ coordinates of the face </returns>
std::vector<glm::vec3> CSynthDefectView::GetPointsFromFace(aiFace const& f)
{
	std::vector<glm::vec3> p;
	p.push_back(m_vertices[f.mIndices[0]].Position);
	p.push_back(m_vertices[f.mIndices[1]].Position);
	p.push_back(m_vertices[f.mIndices[2]].Position);
	return p;
}

/// <summary>
/// Calculate the distance from start point of ray to the surface of loaded model with intersection
/// </summary>
/// </summary>
/// <param name="P">: A point on the triangle </param>
/// <param name="N">" normal of a surface of the triangle </param>
/// <param name="O">: The start point of ray </param>
/// <param name="D">: The direction of the ray </param>
/// <returns> the intersected distance </returns>
float CSynthDefectView::CalculateIntersectedDistance(glm::vec3 const& P, glm::vec3 const& N, glm::vec3 const& O, glm::vec3 const& D)
{
	return (glm::dot(P - O, N) / glm::dot(D, N));
}

/// <summary>
/// Calculate the intersected point with the start point of ray
/// </summary>
/// <param name="dis">: The intersected distance from the start point of ray </param>
/// <param name="O">: The start point of ray </param>
/// <param name="D">: The direction of the ray </param>
/// <returns> The intersection between the start point of ray and the surface of the loaded model </returns>
glm::vec3 CSynthDefectView::CalculateIntersectedPoint(float dis, glm::vec3 const& O, glm::vec3 const& D)
{
	return D * dis + O;
}

/// <summary>
/// Test the intersection is in or on the triangle using normal test
/// </summary>
/// <param name="intersec_p">: The intersection point </param>
/// <param name="A">: First point of the triangle </param>
/// <param name="B">: Second point of the triangle </param>
/// <param name="C">: Third point of the trianlge </param>
/// <returns></returns>
BOOL CSynthDefectView::IsPointOnSurface(glm::vec3 const& P, glm::vec3 const& A, glm::vec3 const& B, glm::vec3 const& C)
{
	BOOL isInA = DoNormalTest(P, A, B, C);
	BOOL isInB = DoNormalTest(P, B, C, A);
	BOOL isInC = DoNormalTest(P, C, A, B);
	return isInA && isInB && isInC;
}

/// <summary>
/// Process the Normal Test
/// </summary>
/// <param name="P">: the intersected point </param>
/// <param name="T">: the target point which is one of points of the triangle to compare normal with the intersected point</param>
/// <param name="A">: another one of points of the triangle </param>
/// <param name="B">: the other one of points of the triangle </param>
/// <returns></returns>
BOOL CSynthDefectView::DoNormalTest(glm::vec3 const& P, glm::vec3 const& T, glm::vec3 const& A, glm::vec3 const& B)
{
	return glm::dot(glm::cross(B - A, P - A), glm::cross(B - A, T - A)) >= 0;
}

/// <summary>
/// Change the color of picked face of the loaded model
/// </summary>
/// <param name="index"> the index of picked face using ray picking </param>
/// <param name="color"> the color that you want to change </param>
void CSynthDefectView::ChangePickedFaceColor(int index, glm::vec4 const& color)
{
	aiFace pickedFace = m_faces[index];
	for (unsigned int i = 0; i < pickedFace.mNumIndices; i++)
	{
		unsigned int index = pickedFace.mIndices[i];
		m_vertices[index].Color = color;
	}
	m_model->UpdateModel(m_vertices);
}

/// <summary>
/// Check the picked face that already is in the vector
/// </summary>
/// <param name="index"> the index of picked face </param>
/// <returns> TRUE if the face is already picked, FALSE otherwise </returns>
BOOL CSynthDefectView::IsPicked(int index)
{
	for (int i = 0; i < m_pickedFaces.size(); i++)
	{
		if (m_pickedFaces.at(i) == index)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/// <summary>
/// Find the duplicated face with the picked one
/// </summary>
/// <param name="index"> the index of picked face </param>
/// <returns> The index of the duplicaed face </returns>
int CSynthDefectView::FindDuplicatedFace(int index)
{
	for (int i = 0; i < m_pickedFaces.size(); i++)
	{
		if (m_pickedFaces.at(i) == index)
			return i;
	}
	return -1;
}

void CSynthDefectView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_camera)
	{
		m_current = glm::vec2((float)point.x, (float)point.y);
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
	delete m_back;
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