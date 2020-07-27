// GlobDrawGL.cpp for implementation Rendering process using OpenGL

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
	glTranslatef(-1.5f, 0.0f, -6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glVertex3f(0.0f, 1.0f, 0.0f);					// Top
		glVertex3f(-1.0f, -1.0f, 0.0f);					// Bottom Left
		glVertex3f(1.0f, -1.0f, 0.0f);					// Bottom Right
	glEnd();											// Finished Drawing The Triangle
	glTranslatef(3.0f, 0.0f, 0.0f);						// Move Right 3 Units
	glBegin(GL_QUADS);									// Draw A Quad
		glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
		glVertex3f(1.0f, 1.0f, 0.0f);					// Top Right
		glVertex3f(1.0f, -1.0f, 0.0f);					// Bottom Right
		glVertex3f(-1.0f, -1.0f, 0.0f);					// Bottom Left
	glEnd();											// Done drawing
	return TRUE;										// Keep going
}