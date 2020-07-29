// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
std::string ConvertStdString(LPCTSTR target)
{
	USES_CONVERSION;
	CString tmpStr = T2CW(target);
	std::string result((CStringA)tmpStr);
	return result;
}