#pragma once

#include <string>

/******************* Constants for Initial Setting *******************/
#define DEFECT_BASE_PATH "../data/base.obj"

std::string ConvertStdString(LPCTSTR target);
std::string RetrieveCode(const char* filePath);