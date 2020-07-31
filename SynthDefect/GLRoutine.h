#pragma once

#include <afxwin.h>
#include <gl/glew.h>
#include <GL/GLU.h>


bool CreateGLWindow(CWnd* pWnd, int bits);
bool KillGLWindow(CWnd* pWnd);
GLvoid ResizeGLScene(GLsizei width, GLsizei height);
