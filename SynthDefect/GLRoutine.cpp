// GLRoutine.cpp for the common Global OpenGL Procedures


#include "GLRoutine.h"


/// <summary>
/// Properly Kill the Window
/// </summary>
/// <param name="pWnd"> the client area of a specified window or for the entire screen </param>
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
	// wglMakeCurrent - Make a specified OpenGL Rendering Context the calling thread's Current Rendering Context.
	//                  Use to change the calling thread's Current Rendering Context so it's no longer current.                  
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