#pragma once

#include <string>
#include <array>

/******************* Constants for Initial Setting *******************/

#define DEFECT_BASE_PATH "../data/base.obj"

/******************* User-Defined Window Messages *******************/

// For PropertiesWindow
#define UM_UPDATE_PROPERTIES	WM_USER + 100

// For MainFrame
#define UM_GET_PROPERTIES		WM_USER + 200
#define UM_GENERATE_ERROR		WM_USER + 201

// For View
#define UM_EXECUTE_PIPELINE		WM_USER + 300

/******************* Structure for Properties *******************/

struct Properties {
	int type = 0;
	float width = 0.0f;
	float height = 0.0f;
	float depth = 0.0f;
	int numOfPoints = 0;
};

std::string ConvertStdString(LPCTSTR target);
std::string RetrieveCode(const char* filePath);