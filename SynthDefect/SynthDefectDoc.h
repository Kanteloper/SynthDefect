
// SynthDefectDoc.h : interface of the CSynthDefectDoc class
//


#pragma once

#include "Model.h"



class CSynthDefectDoc : public CDocument
{
protected: // create from serialization only
	CSynthDefectDoc() noexcept;
	DECLARE_DYNCREATE(CSynthDefectDoc)

// Attributes
private:
	CView* pView = nullptr;
	glm::vec3 m_back = glm::vec3(3.0f, 1.4f, 0.0f);				// default background vertex	
public:
	CModel* m_model = nullptr;

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
