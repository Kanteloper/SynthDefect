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

// Light position
const GLfloat LIGHT_X = 0.0f;
const GLfloat LIGHT_Y = 0.0f;
const GLfloat LIGHT_Z = 40.0f;

// Light color
const GLfloat LIGHT_R = 1.0f;
const GLfloat LIGHT_G = 1.0f;
const GLfloat LIGHT_B = 1.0f;

/******************* Constants for Camera *******************/

// Zoom
const GLfloat MAX_ZOOM = 180.0f;
const GLfloat MIN_ZOOM = 5.0f;
const GLfloat ZOOM_OFFSET = 4.0f;
const GLfloat FOV = 45.0f;

// Rotation
const GLfloat ROTATION_OFFSET = 0.06f;
const GLfloat SENSITIVITY = 0.5f;
const GLfloat TRACKBALL_RADIUS = 1.0f;
const GLint REFRESH = 100;

// Default setting
const GLfloat DISTANCE_FROM_MODEL = 40.0f;


bool CreateGLWindow(CWnd* pWnd, int bits);
bool KillGLWindow(CWnd* pWnd);
GLvoid ResizeGLScene(GLsizei width, GLsizei height);
void InitializeGLEngine();
void SetWireFrameMode(BOOL set);
