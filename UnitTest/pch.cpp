// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
CString getFileFormat(const char* path)
{
	int curPos = 0;
	CString tmpStr(path);
	curPos = tmpStr.Find(_T("."));
	tmpStr = tmpStr.Tokenize(_T("."), curPos);
	return tmpStr;
}

FILE* MyFileOpen(_TCHAR* fPath, _TCHAR* mode)
{
	FILE* fp = NULL;
	errno_t err = _tfopen_s(&fp, fPath, mode);
	if (!err)
		return fp;
	else
		return NULL;
}