
// SynthDefectDoc.cpp : implementation of the CSynthDefectDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SynthDefect.h"
#endif

#include "SynthDefectDoc.h"
#include "SynthDefectView.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSynthDefectDoc

IMPLEMENT_DYNCREATE(CSynthDefectDoc, CDocument)

BEGIN_MESSAGE_MAP(CSynthDefectDoc, CDocument)
END_MESSAGE_MAP()


// CSynthDefectDoc construction/destruction

CSynthDefectDoc::CSynthDefectDoc() noexcept
{
	// TODO: add one-time construction code here
}


CSynthDefectDoc::~CSynthDefectDoc()
{
	delete m_model;
}


/// <summary>
/// Called by the framework as part of the File 'New' command
/// </summary>
/// <returns> Nonzero if the document was successfully initialized; otherwise 0</returns>
// If the users chooses the File 'New' command in an SDI application, 
// the framework uses this function to 'reintialize' the existing document, rather than creating a new one.
// Must place 'initialization' code in this function instead of in the constructor
// for the File 'New' command to be effective in SDI applications.
BOOL CSynthDefectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

		m_model = new CModel(m_back);

	return TRUE;
}


/// <summary>
/// Get the reference of View object
/// </summary>
/// <returns> the pointer of CView object </returns>
CView* CSynthDefectDoc::GetChildView()
{
	CView* target = nullptr;
	POSITION pos = GetFirstViewPosition();
	if (pos)
		target = GetNextView(pos);
	return target;
}


/// <summary>
/// Called by the framework as part of the File 'Open' command
/// </summary>
/// <param name="lpszPathName">: Points to the path of the document to be opened</param>
/// <returns>Nonzero if the document was successfully loaded; otherwise 0</returns>
// LPCTSTR - Long Point Constant Typecasting String
BOOL CSynthDefectDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// load model
	m_model = new CModel(lpszPathName, m_back);
	m_bLoad = TRUE;
	TRACE3("Log: maxX - %f, maxY - %f, maxZ - %f\n", m_model->m_max.x, m_model->m_max.y, m_model->m_max.z);
	TRACE3("Log: minX - %f, minY - %f, minZ - %f\n", m_model->m_min.x, m_model->m_min.y, m_model->m_min.z);

	return TRUE;
}



// CSynthDefectDoc serialization - not used
void CSynthDefectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{

	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSynthDefectDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSynthDefectDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSynthDefectDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSynthDefectDoc diagnostics

#ifdef _DEBUG
void CSynthDefectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSynthDefectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSynthDefectDoc commands