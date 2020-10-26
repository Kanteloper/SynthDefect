#pragma once

#include <string>

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

/******************* Constants for Pipelines *******************/

// Deformation - lattice space
const int S_ORDER = 6;
const int S_DEGREE = 3;
const float S_AXIS_SIZE = 2.4f;
const int T_ORDER = 1;
const int T_DEGREE = 1;
const float T_AXIS_SIZE = 2.4f;
const int U_ORDER = 6;
const int U_DEGREE = 3;
const float U_AXIS_SIZE = 2.4f;

// Deformation - Key control points of each defect
const float MAX_OPEN_HOLE_Y = -0.1f;
const float MIN_OPEN_HOLE_Y = -0.5f;
const float MAX_PIPE_MID_Y = -0.5f;
const float MIN_PIPE_MID_Y = -1.0f;
const float MAX_PIPE_SIDE_Y = -0.1f;
const float MIN_PIPE_SIDE_Y = -0.5f;
const float MAX_CAVED_SURFACE_MID_Y = -0.1f;
const float MIN_CAVED_SURFACE_MID_Y = -0.15f;
const float MAX_CAVED_SURFACE_SIDE_Y = -0.05f;
const float MIN_CAVED_SURFACE_SIDE_Y = -0.1f;

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