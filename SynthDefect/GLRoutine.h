#pragma once

#include <afxwin.h>
#include <gl/glew.h>
#include <GL/GLU.h>

#define ID_TIMER_PLAY 100								// Unique ID of system timer for a specific window

/******************* Constants for GLSL *******************/

#define VS_BACKGROUND_PATH	"./background_vs.glsl"		// The current path of vertex shader code file for background mesh
#define FS_BACKGROUND_PATH	"./background_fs.glsl"		// The current path of fragment shader code file for background mesh
#define VS_MODEL_PATH		"./model_vs.glsl"			// The current path of fragment shader code file for background mesh
#define FS_MODEL_PATH		"./model_fs.glsl"			// The current path of fragment shader code file for background mesh

/******************* Constants for Workspace *******************/

const float WORKSPACE_WIDTH = 16.0f;					// the width of workspace screen
const float WORKSPACE_HEIGHT = 2.0f;					// the height of workspace screen
const float WORKSPACE_X = 5.0f;							// the x value for workspace	
const float WORKSPACE_Y = 1.2f;							// the y value for workspace
const float WORKSPACE_Z = -1.0f;							// the z value for workspace

/******************* Constants for Light Source *******************/

const float LIGHT_X = 0.0f;
const float LIGHT_Y = 1.0f;
const float LIGHT_Z = 0.0f;

bool CreateGLWindow(CWnd* pWnd, int bits);
bool KillGLWindow(CWnd* pWnd);
GLvoid ResizeGLScene(GLsizei width, GLsizei height);
void InitializeGLEngine();
