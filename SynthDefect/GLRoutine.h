#pragma once

#include <afxwin.h>
#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>

bool CreateGLWindow(CWnd* pWnd, int bits);
bool KillGLWindow(CWnd* pWnd);
GLvoid InitGLScene(GLsizei width, GLsizei height);
