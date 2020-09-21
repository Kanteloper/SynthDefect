// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include "GLRoutine.h"
#include "GlobMyProc.h"

/*********** User-Define Window Messages ***********/

// For PropertiesWindow
#define UM_UPDATE_PROPERTIES	WM_USER + 100

// For MainFrame
#define UM_GET_PROPERTIES		WM_USER + 200
#define UM_GENERATE_ERROR		WM_USER + 201

// For View
#define UM_EXECUTE_PIPELINE		WM_USER + 300




struct DefectProperties {
	int num = 0;
	// type
	// size
};

#endif //PCH_H
