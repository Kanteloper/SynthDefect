
// SynthDefectView.h : interface of the CSynthDefectView class
//

#pragma once
#include "Shader.h"
#include "Camera.h"

class CSynthDefectView : public CView
{
protected: // create from serialization only
	CSynthDefectView() noexcept;
	DECLARE_DYNCREATE(CSynthDefectView)

// Attributes
public:
	float m_viewWidth = 0.0f;
	float m_viewHeight = 0.0f;
private:
	/* Flag */
	BOOL m_bInitGL = TRUE;
	BOOL m_bWireframe = FALSE;

	/* Model */
	CShader m_backgroundShader, m_modelShader;
	CModel* m_model = nullptr;

	/* Render */
	float m_scaleFactor = 0.0f;
	glm::vec3 m_modelCenter;
	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_modelMatrix;

	/* Camera */
	CCamera* m_camera = nullptr;
	glm::vec3 m_cameraPos;
	glm::vec2 m_current;
	glm::vec2 m_later;

	/* Ray Picking */
	glm::vec3 ray_start;
	glm::vec3 ray_dir;

public:
	CSynthDefectDoc* GetDocument() const;

// Overrides
public:
	virtual void OnDraw(CDC* pDC); // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void InitChildView();

// Implementation
private:
	BOOL DrawGLScene();
	void DrawBackground();
	void DrawLoadedModel();
	void InitSettings();
	glm::vec3 CalculateMouseRay(CPoint const& p);
	glm::vec2 GetNormalizedDeviceCoords(CPoint const& p);
	glm::vec4 toEyeCoords(glm::vec4 const& clip);
	glm::vec3 toWorldCoords(glm::vec4 const& eye);
	float CalculateIntersectedDistance(std::vector<aiFace> const& faces, std::vector<Vertex> const& vertices);
	float CalculateDistanceToSurface(std::vector<glm::vec3> const& points);


public:
	virtual ~CSynthDefectView();
	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL DestroyWindow();
	virtual void OnInitialUpdate();
	virtual void PostNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in SynthDefectView.cpp
inline CSynthDefectDoc* CSynthDefectView::GetDocument() const
   { return reinterpret_cast<CSynthDefectDoc*>(m_pDocument); }
#endif

