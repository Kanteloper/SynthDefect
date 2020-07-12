
#include "pch.h"

// All Setup for OpenGL
void InitGL()
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing

	// The Type Of Depth Testing To Do
	// GL_LEQUAL : Passes if the incoming depth value is less than or equal to the stored depth value.
	glDepthFunc(GL_LEQUAL);

	// Specify implementation-specific hints
	// Certain aspects of GL behavior can be controlled with hints.
	// GL_PERSPECTIVE_CORRECTION_HINT: Indicates the quality of color, texture coordinate, and fog(¾È°³) coordinate interpolation.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

int DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset the current ModelView Matrix
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();											// Done drawing
	return TRUE;										// Keep going
}