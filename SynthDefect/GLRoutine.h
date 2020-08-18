#pragma once

#include <afxwin.h>
#include <gl/glew.h>
#include <GL/GLU.h>

#define ID_TIMER_PLAY 100								// Unique ID of system timer for a specific window

/******************* Constants for GLSL *******************/

#define VS_BACKGROUND_PATH	"./background_vs.glsl"		// The current path of vertex shader code file for background mesh
#define FS_BACKGROUND_PATH	"./background_fs.glsl"		// The current path of fragment shader code file for background mesh
#define VS_MODEL_PATH		"./model_vs.glsl"			// The current path of vertex shader code file for model mesh
#define FS_MODEL_PATH		"./model_fs.glsl"			// The current path of fragment shader code file for model mesh

/******************* Constants for Light Source *******************/

// light position
const float LIGHT_X = 0.0f;
const float LIGHT_Y = 0.0f;
const float LIGHT_Z = 40.0f;

// light color
const float LIGHT_R = 1.0f;
const float LIGHT_G = 1.0f;
const float LIGHT_B = 1.0f;

/******************* Constants for Camera *******************/

const float MAX_ZOOM = 180.0f;
const float MIN_ZOOM = 5.0f;
const float ZOOM_OFFSET = 4.0f;
const float ROTATION_OFFSET = 0.06f;


bool CreateGLWindow(CWnd* pWnd, int bits);
bool KillGLWindow(CWnd* pWnd);
GLvoid ResizeGLScene(GLsizei width, GLsizei height);
void InitializeGLEngine();
