
// SynthDefectDoc.h : interface of the CSynthDefectDoc class
//


#pragma once

#include "Model.h"
#include "Base.h"


class CSynthDefectDoc : public CDocument
{
protected: // create from serialization only
	CSynthDefectDoc() noexcept;
	DECLARE_DYNCREATE(CSynthDefectDoc)

// Attributes
private:
	BOOL m_bLoad;					// the flag for checking the model is loaded
	BOOL m_bInitModel;				// the flag for checking the model is loaded at first

public:
	CModel* m_model;				// the loaded model
	CBase* m_base;					// the base mesh of synthetic defect

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
private:
	CView* GetChildView();
public:
	virtual ~CSynthDefectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
