#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

CString getFileFormat(const char* path);
FILE* MyFileOpen(_TCHAR* fPath, _TCHAR* mode);
BOOL LoadModel(LPCTSTR pathName);
std::string ConvertStdString(LPCTSTR target);