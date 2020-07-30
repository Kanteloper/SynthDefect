
// SynthDefectView.h : interface of the CSynthDefectView class
//

#pragma once


class CSynthDefectView : public CView
{
protected: // create from serialization only
	CSynthDefectView() noexcept;
	DECLARE_DYNCREATE(CSynthDefectView)

// Attributes
public:
	CSynthDefectDoc* GetDocument() const;
	bool m_bInitGL;
	CWnd m_wndChild;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void InitChildView();

protected:

// Implementation
public:
	virtual ~CSynthDefectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
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
};

#ifndef _DEBUG  // debug version in SynthDefectView.cpp
inline CSynthDefectDoc* CSynthDefectView::GetDocument() const
   { return reinterpret_cast<CSynthDefectDoc*>(m_pDocument); }
#endif

