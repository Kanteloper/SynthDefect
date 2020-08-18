
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
	BOOL m_bInitGL = TRUE;

	/* Model */
	float m_scaleFactor = 0.0f;
	CShader m_backgroundShader, m_modelShader;
	CModel* m_model = nullptr;
	glm::vec3 m_modelCenter;

	/* Camera */
	CCamera* m_camera = nullptr;
	glm::vec3 m_cameraPos;
	float m_angleX = 0.0f;
	float m_angleY = 0.0f;
	float m_currentX = 0.0f;
	float m_currentY = 0.0f;

public:
	CSynthDefectDoc* GetDocument() const;

// Overrides
public:
	virtual void OnDraw(CDC* pDC); // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void InitChildView();

protected:

// Implementation
private:
	BOOL DrawGLScene();
	void DrawBackground();
	void DrawLoadedModel();
	glm::vec3 GetModelCentroid(glm::vec3 max, glm::vec3 min);
	float GetScaleFactor(glm::vec3 max, glm::vec3 min, glm::vec3 center);
	void InitializeCamera();
public:
	virtual ~CSynthDefectView();
	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
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
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in SynthDefectView.cpp
inline CSynthDefectDoc* CSynthDefectView::GetDocument() const
   { return reinterpret_cast<CSynthDefectDoc*>(m_pDocument); }
#endif

