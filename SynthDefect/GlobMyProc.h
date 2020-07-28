#pragma once

CString getFileFormat(const char* path);
FILE* MyFileOpen(_TCHAR* fPath, _TCHAR* mode);
BOOL LoadObject(LPCTSTR pathName);