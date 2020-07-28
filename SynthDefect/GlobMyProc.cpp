// GlobMyProc.cpp for common Global utility procedures

#include "pch.h"

/// <summary>
/// Open a file of target path
/// </summary>
/// <param name="fPath">: the path of a file</param>
/// <param name="mode">: type of access permitted</param>
/// <returns> the pointer of FILE if success; NULL on failure </returns>
FILE* MyFileOpen(_TCHAR* fPath, _TCHAR* mode)
{
	FILE* fp = NULL;
	errno_t err = _tfopen_s(&fp, fPath, mode);
	if (!err)
		return fp;
	else
		return NULL;
}


/// <summary>
/// extract file format from file path
/// </summary>
/// <param name="path">: Relative or Absolute path of selected file </param>
/// <returns> file </returns>
CString getFileFormat(LPCTSTR path)
{
	int curPos = 0;

	// In order to for the macros to have space to store the a temporary length, 
	// it is necessary to declare a local variable called '_convert'
	// See https://docs.microsoft.com/ko-kr/cpp/mfc/tn059-using-mfc-mbcs-unicode-conversion-macros?view=vs-2019
	// convert lpctstr(MBCS) to lpcstr(MBCS), convert lpcstr(MBCS) to CString
	USES_CONVERSION;
	CString tmpStr = T2CW(path);
	curPos = tmpStr.Find(_T("."));
	tmpStr = tmpStr.Tokenize(_T("."), curPos);
	return tmpStr;
}


/// <summary>
/// Load selected object file
/// </summary>
/// <param name="pathName">: the path of selected file </param>
/// <returns> TRUE if load success, FALSE on failure </returns>
BOOL LoadObject(LPCTSTR pathName)
{
	// Check file format
	CString fileFormat = getFileFormat(pathName);

	// Possible to add other fileformat of Mesh
	if (fileFormat == "obj")
	{
		FILE* file;
		file = MyFileOpen((LPWSTR)pathName, _T("r"));

		while (1)
		{
			TCHAR lineHeader[128];
			int res;
			// Read the first word of the line
			if ((res = _ftscanf_s(file, _T("%s"), lineHeader, _countof(lineHeader))) == EOF)
				break;

			// Parse line header
		}
	}
	else
	{
		//AfxMessageBox(L"Not support \'" fileFormat , MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}