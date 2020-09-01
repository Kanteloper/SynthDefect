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
#define VS_BASE_PATH		"./base_vs.glsl"			// The current path of vertex shader code file for base plane mesh
#define FS_BASE_PATH		"./base_fs.glsl"			// The current path of fragment shader code file for base plane mesh

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
const GLfloat CAMERA_X = -20.0f;
const GLfloat CAMERA_Y = 30.0f;
const GLfloat CAMERA_Z = 40.0f;
const GLfloat MAX_RAY_DISTANCE = 600.0f;

/******************* Constants for Light Source *******************/

// Light position
const GLfloat LIGHT_X = CAMERA_X;
const GLfloat LIGHT_Y = CAMERA_Y;
const GLfloat LIGHT_Z = CAMERA_Z;

// Light color
const GLfloat LIGHT_R = 1.0f;
const GLfloat LIGHT_G = 1.0f;
const GLfloat LIGHT_B = 1.0f;


/******************* Constants for Base plane *******************/
const GLint VERTEX_NUM = 64;
const GLint BASE_SIZE = 150;

bool CreateGLWindow(CWnd* pWnd, int bits);
bool KillGLWindow(CWnd* pWnd);
GLvoid ResizeGLScene(GLsizei width, GLsizei height);
void InitializeGLEngine();
void SetWireFrameMode(BOOL set);
