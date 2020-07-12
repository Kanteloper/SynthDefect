// GLRoutine.cpp for the common Global OpenGL Procedures

#include "pch.h"

/// <summary>
/// Create OpenGL window
/// </summary>
/// <param name="pWnd">: the client area of a specified window </param>
/// <param name="bits">: the size of Color buffer(= Color Depth)</param>
/// <returns> bool </returns>
bool CreateGLWindow(CWnd* pWnd, int bits)
{
	// pfd Tells Windows How We Want Things To Be
	// PIXELFORMATDESCRIPTOR structure describes the pixel format of a drawing surface.
	static PIXELFORMATDESCRIPTOR pfd =					
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor structure
		1,												// Version Number(fixed)
		/************************************************************************************************
		 * dwFlags - A big flag that specify properties of the pixel buffer
		 ************************************************************************************************/
		// PFD_DRAW_TO_WINDOW - The buffer can draw to a window or device surface.
		PFD_DRAW_TO_WINDOW |							// Format Must Support Window
		// PFD_SUPPORT_OPENGL - The bufffer supports OpenGL drawing.
		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
		// PFD_DOUBLEBUFFER - The buffer is double-buffered.
		PFD_DOUBLEBUFFER,								// Must Support Double Buffering

		/************************************************************************************************
		 * iPixelType - Specifies the type of pixel data.
		 ************************************************************************************************/
		PFD_TYPE_RGBA,									// Request An RGBA Format
		/************************************************************************************************
		 * cColorBits - Specifies the number of color bitplanes in each color buffer
		 *              For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes.
		 ************************************************************************************************/
		(BYTE)bits,										// Select Our Color Depth
		/************************************************************************************************
		 * (cRedBits, cRedShift), (cGreenBits, cGreenShift), (cBlueBits, cBlueShift)
		 ************************************************************************************************/
		0, 0, 0, 0, 0, 0,								// Color Bits Ignored
		/************************************************************************************************
		 * (cAlphaBits, cAlphaShift)
		 ************************************************************************************************/
		0, 0,											// Alpha Bits Ignored
		0,												// No Accumulation Buffer
		0, 0, 0, 0,										// Accumulation Bits Ignored (R, G, B, A)
		16,												// 16Bit Z-Buffer (Depth Buffer)  
		/************************************************************************************************
		 * cStencilBits - Specifies the depth of the stencil buffer
		 ************************************************************************************************/
		1,												// Use Stencil Buffer ( * Important * )
		0,												// No Auxiliary Buffer
		0,												// Layer Type Ignored
		0,												// Reserved
		0, 0, 0											// Layer Masks Ignored
	};

	HDC hDC; // Handle Device Context

	// GetDC - Retrieve a handle to a device context for the client area of a specified window or for the entire screen
	//		   Retrieve a Common, Class or Private DC depending on the class style of the specified window
	if (!(hDC = GetDC(pWnd->m_hWnd)))						
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL, TEXT("Can't Create A GL Device Context."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;										// Return FALSE
	}

	int iPixelFormat;
	// Attempt to match an appropriate pixel format supported by a device context to a given pfd
	if (!(iPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL, TEXT("Can't Find A Suitable PixelFormat."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;	
	}

	// Set the pixel format of the specified Device Context to the format specified by the iPixelFormat index
	if (!SetPixelFormat(hDC, iPixelFormat, &pfd))
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL, TEXT("Can't Set The PixelFormat."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;		
	}

	HGLRC hRC;												// Handle Rendering Context
	// Create a new OpenGL Rendering Context, which is suitable for drawing on the device referenced by hdc
	if (!(hRC = wglCreateContext(hDC)))					 
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL, TEXT("Can't Create A GL Rendering Context."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							
	}

	// Make a specified OpenGL Rendering Context the calling thread's Current Rendering Context
	if (!wglMakeCurrent(hDC, hRC))
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL, TEXT("Can't Activate The GL Rendering Context."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									
	}

	CRect rect;
	// GetClientRect - Retrieve the coordinates of a window's client area
	// The client coordinates specify the upper-left and lower-right corners of the client area.
	// The coordinates of the upper-left corner are (0,0).
	pWnd->GetClientRect(&rect);
	// SetForegroundWindow - Bring the thread that created the specified window into the foreground
	//                       and activates the window
	// The system assigns a slightly higher priority to the thread that created the foreground window
	SetForegroundWindow(pWnd->m_hWnd);
	ResizeGLScene(rect.Width(), rect.Height());				// Set Up Our Perspective GL Screen

	return TRUE;
}

/// <summary>
/// Properly Kill the Window
/// </summary>
/// <param name="pWnd">: the client area of a specified window </param>
/// <returns> bool </returns>
bool KillGLWindow(CWnd* pWnd)
{
	if (pWnd == NULL)
	{
		// MessageBox - Display a modal dialog box that contains system icon, a set of buttons,
		//				and a brief application-specific message.
		// See Details https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
		MessageBox(NULL, TEXT("KillGLWindow\npWnd == NULL"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	HWND hWnd = pWnd->m_hWnd;
	// See Detail aboud Private Device Context https://docs.microsoft.com/en-us/windows/win32/gdi/private-display-device-contexts
	HDC	hDC = GetDC(hWnd);								// Get Private GDI Device Context
	// wglCreateContext - Create a new OpenGL Rendering Context, which is suitable for drawing
	//                    on the device referenced by hDC.
	//					  The Rendering Context has the same pixel format as the device context.
	HGLRC hRC = wglCreateContext(hDC);					// Handle Rendering Context

	if (!hRC) 
		return FALSE;
	// wglMakeCurrent - Make a specified OpenGL Rendering Context the calling thread's Current Rendering Context
	//                  Use to change the calling thread's Current Rendering Context so it's no longer current                 
	// See details about Rendering Context https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-wglmakecurrent
	// See also https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
	if (!wglMakeCurrent(NULL, NULL)) // check the context to delete is not current
	{
		MessageBox(NULL, TEXT("Release Of DC And RC Failed."), TEXT("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	// Now that the context is not current, Be able to call wglDeleteContext

	if (!wglDeleteContext(hRC))							// Delete a specified OpenGL Rendering Context
	{
		MessageBox(NULL, TEXT("Release Rendering Context Failed."), TEXT("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	hRC = NULL;											// Set RC To NULL

	// ReleaseDC - Release a Device Context. It frees only Common DCs and Window DCs.
	//			   It has no effect on Class DCs or Private DCs.	
	//			   It must be called to release the DC.
	//		       It must be called from the same thread that called GetDC.
	if (hDC && !ReleaseDC(hWnd, hDC))					
	{
		MessageBox(NULL, TEXT("Release Device Context Failed."), TEXT("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
		return FALSE;
	}

	return TRUE;
}

GLfloat fNearPlane = 0.1f, fFarPlane = 100.f, fViewAngle = 45.f, fAspect;
RECT m_viewRect;

/// <summary>
/// Resize and Initialize the OpenGL window
/// </summary>
/// <param name="width">: width of OpenGL window </param>
/// <param name="height">: height of OpenGL window </param>
/// <returns></returns>
GLvoid ResizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0)									// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}
	m_viewRect.left = m_viewRect.top = 0;				// The coordinates of the upper-left corner are (0,0).
	m_viewRect.right = width;
	m_viewRect.bottom = height;

	glViewport(0, 0, width, height);					// Reset The Current Viewport

	// glMatrixMode - Specify which matrix is the current matrix
	// GL_PROJECTION - Apply subsequent matrix operations to the projection matrix stack
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix to Identity Matrix

	// gluPerspective - Set up a Perspective Projection Matrix
	// Specify a Viewing frustrum into the World coordinate system
	gluPerspective(fViewAngle, (GLfloat)width / (GLfloat)height, fNearPlane, fFarPlane);  

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}