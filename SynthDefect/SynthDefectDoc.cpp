
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
#include <gl/GL.h>
#include <gl/GLU.h>
#include <propkey.h>
#include <cstdio>

#include <iostream>

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

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetModifiedFlag(TRUE);

	return TRUE;
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
	
	// In order to for the macros to have space to store the a temporary length, 
	// it is necessary to declare a local variable called '_convert'
	// See https://docs.microsoft.com/ko-kr/cpp/mfc/tn059-using-mfc-mbcs-unicode-conversion-macros?view=vs-2019
	USES_CONVERSION;
	// convert lptstr(MBCS) to unicode(const char*)
	const char* filePath = W2A(lpszPathName);

	// Check file format
	CString fileFormat = getFileFormat(filePath);
	// Possible to add other fileformat of Mesh
	if (fileFormat == "obj")
	{
		FILE* fp;
		errno_t err = fopen_s(&fp, filePath, "r");
		ASSERT(err == 0);

		while (1)
		{
			char lineHeader[128];
			int res;
			// Read the first word of the line
			if ((res = fscanf_s(fp, "%s", lineHeader, _countof(lineHeader))) == EOF)
				break;

			// Parse line header
		}
	}
	else
	{
		//AfxMessageBox(L"Not support \'" fileFormat , MB_OK | MB_ICONERROR);
	}

	return TRUE;
}


/// <summary>
/// extract file format from file path
/// </summary>
/// <param name="path">: Relative or Absolute path of selected file </param>
/// <returns> file </returns>
CString CSynthDefectDoc::getFileFormat(const char* path)
{
	int curPos = 0;
	CString tmpStr(path);
	curPos = tmpStr.Find(_T("."));
	tmpStr = tmpStr.Tokenize(_T("."), curPos);
	return tmpStr;
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
		// TODO: add loading code here
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


