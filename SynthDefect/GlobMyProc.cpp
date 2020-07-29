// GlobMyProc.cpp for common Global utility procedures

#include "pch.h"

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