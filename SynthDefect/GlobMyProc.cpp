// GlobMyProc.cpp for common Global utility procedures

#include "pch.h"

/// <summary>
/// Load selected object file
/// </summary>
/// <param name="pathName">: the path of selected file </param>
/// <returns> TRUE if load success, FALSE on failure </returns>
BOOL LoadModel(LPCTSTR pathName)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(ConvertStdString(pathName), aiProcess_Triangulate | aiProcess_FlipUVs);


	return TRUE;
}


/// <summary>
/// Convert LPCTSTR to std::string
/// </summary>
/// <param name="target">: Target LPCTSTR of conversion </param>
/// <returns> Converted value of std::string type </returns>
std::string ConvertStdString(LPCTSTR target)
{
	// In order to for the macros to have space to store the a temporary length, 
	// See https://docs.microsoft.com/ko-kr/cpp/mfc/tn059-using-mfc-mbcs-unicode-conversion-macros?view=vs-2019
	// convert lpctstr(MBCS) to lpcstr(MBCS), convert lpcstr(MBCS) to CString
	USES_CONVERSION;
	CString tmpStr = T2CW(target);
	std::string result((CStringA)tmpStr);
	return result;
}