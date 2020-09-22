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

/******************* Constants for Camera *******************/

// Projection
const GLfloat NEAR_PLANE = 0.5f;
const GLfloat FAR_PLANE = 100.0f;

// Zoom
const GLfloat MIN_ZOOM = 180.0f;
const GLfloat MAX_ZOOM = 1.0f;
const GLfloat ZOOM_OFFSET = 2.0f;
const GLfloat FOV = 45.0f;

// Rotation
const GLfloat ROTATION_OFFSET = 0.06f;
const GLfloat SENSITIVITY = 4.0f;
const GLfloat TRACKBALL_RADIUS = 1.0f;
const GLint REFRESH = 100;

// Default setting
const GLfloat CAMERA_X = 0.0f;
const GLfloat CAMERA_Y = 0.0f;
const GLfloat CAMERA_Z = 40.0f;


/******************* Constants for Ray Picking *******************/

const float MAX_DISTANCE = 100000.0f;
const GLfloat RAY_OFFSET = -1.03f;
const GLfloat PICK_R = 1.0f;
const GLfloat PICK_G = 0.68f;
const GLfloat PICK_B = 0.0f;
const GLfloat PICK_A = 0.0f;

/******************* Constants for Light Source *******************/

// Light position
const GLfloat LIGHT_X = CAMERA_X;
const GLfloat LIGHT_Y = CAMERA_Y;
const GLfloat LIGHT_Z = CAMERA_Z;

// Light color
const GLfloat LIGHT_R = 1.0f;
const GLfloat LIGHT_G = 1.0f;
const GLfloat LIGHT_B = 1.0f;

/******************* Constants for Model *******************/

// Model color
const GLfloat MODEL_R = 0.5f;
const GLfloat MODEL_G = 0.5f;
const GLfloat MODEL_B = 0.5f;
const GLfloat MODEL_A = 0.0f;

/******************* Constants for Background *******************/

// Background color
const GLfloat BACK_TOP_R = 0.04f;
const GLfloat BACK_TOP_G = 0.4f;
const GLfloat BACK_TOP_B = 0.6f;
const GLfloat BACK_BOTTOM_R = 0.0f;
const GLfloat BACK_BOTTOM_G = 0.0f;
const GLfloat BACK_BOTTOM_B = 0.0f;
const GLfloat BACK_A = 1.0f;

bool CreateGLWindow(CWnd* pWnd, int bits);
bool KillGLWindow(CWnd* pWnd);
GLvoid ResizeGLScene(GLsizei width, GLsizei height);
void InitializeGLEngine();
void SetWireFrameMode(BOOL set);
