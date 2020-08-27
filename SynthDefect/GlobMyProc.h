#pragma once

#include <string>

/******************* Constants for Ray Picking *******************/

const float MAX_DISTANCE = 100000.0f;

std::string ConvertStdString(LPCTSTR target);
std::string RetrieveCode(const char* filePath);