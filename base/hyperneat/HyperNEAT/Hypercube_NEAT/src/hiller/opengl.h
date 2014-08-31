#pragma once
//#include <gl/gl.h>
//#include <gl/glu.h>
//#include "GLUtils.h"
#include <GLUT/glut.h>


//******************TODO: (on implementation)*************************//
// Class by Jon Hiller: (jdh74@cornell.edu) (Updated Oct 2008)
//
// 1) Add libraries: Right-click project->Properties->linker->input->Additional dependencies: "opengl32.lib glu32.lib"
//
// 2) #include "OpenGL.h" in view class (or any other window class you want to use) header
//
// 3) Add a member in view class, ex: COpenGL m_GLWindow; (Do this towards the end of the file or you'll get LINK2005 error)
//
// 4) Overload the OnCreate function of the view class. Using visual studio, select your view class in the class view, open the properties, click on "messages" tab, find "WM_CREATE", and click to the right to overload it.
// You can do this manually, just don't forget to add ON_WM_CREATE() to the message map of your view class.

// 5) In the OnCreate function, add in the following lines to create the OpenGL Window and sync up the view class pointer: 
//	m_GLWindow.Create(NULL, "OpenGL Window", WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, IDR_OpenGL_Window, 0);
//  m_GLWindow.Sync(this);
//
// 6) Overload the OnSize function of the view class (see item 4, look for WM_SIZE), add in the following line to pass in the resize:
//	m_GLWindow.MoveWindow(&CRect(0,0,cx,cy));
//
// 7) Also, in the Resource.h file, add in a line like #define IDR_OpenGL_Window 9989, where 9989 is completely arbitrary **seems to get overwritten. forget label.
//
// 8) Find and Replace all the "COpenGL_DemoView" instances with the name of your view class
//
// 9) **OPTIONAL: IF THE MOUSEWHEEL DO NOT CAUSE TO VIEW TO ZOOM: 
//   Override OnMouseWheel in view class (see item 4, look for WM_MOUSEWHEEL), add 
//   m_GLWindow.OnMouseWheel(nFlags, zDelta, pt);
//
// 10) **OPTIONAL: IF YOU PLAN ON USING GLUTILS OR VEC FUNCTIONALITY ELSEWHERE:
//  Using GLUtils and Vec, add the header & source files to project, include headers in stdafx.h: (or in individual classes you plan to use them in)
//  #include "GLUtils.h"
//  #include "Vec.h"
//
//*****************End TODO: (on implementation)***********************//

class COpenGL_DemoView; //TODO: change this to you view class name

class COpenGL : public CWnd
{

//Variables

private: //OpenGL variables
	HDC		hdc;
	HGLRC	hrc;
	int		m_nPixelFormat;
	CRect	m_WindowRect;
	
public:
	UINT_PTR m_GLTimer; //For use with a locked framerate

	//Camera parameters structure:
	struct CAMERA
	{
		GLfloat	Xpos; //Actual position of the camera
		GLfloat	Ypos;
		GLfloat	Zoom;
		GLfloat	Xrot; //rotations of the camera
		GLfloat Yrot;
		GLfloat	Persp; //Amount of persective
		GLfloat TargetX; //Target of the camera (Also the point the view rotates around)
		GLfloat TargetY;
		GLfloat TargetZ;
	};

	CAMERA m_Cam;	//Camera Position
	CAMERA m_TCam;	//Target Camera position (for animating camera)

	GLfloat m_fLastX; //point coordinate storage to get mouse movement
	GLfloat m_fLastY;

	bool bTimer; //Enable/disable locked framerate

	bool bDrawBounds; //choose to draw bounds
	bool bDrawFloor; //choose to draw Floor
	bool bDrawAxes; //choose to draw Axes

	COpenGL_DemoView* mp_View; //TODO: change this to your view class name

//Functions
public:
	COpenGL(void);
	~COpenGL(void);
	void GLInit();
	void Sync(COpenGL_DemoView* p_View); //TODO: change this to your view class name
	void GLSetPersp(void);
	void GLSetLighting(void);
	void GLDrawScene(void);
	void GLRedraw(void);

	//Drawing functions:
	void GLDrawBounds(void); //Draw a generic bounding box
	void GLDrawAxes(bool Line); //Draw axes with Lines (true) or shapes (false)
	void GLDrawFloor(void); //Draw a tiled floor


//Message map
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

};
