#include "OpenGL.h"
//#include "GLUtils.h"

class CGLUtils;


//***********************************************************************************
//Constructor
//***********************************************************************************
COpenGL::COpenGL(void)
{
	mp_View = 0; //initialize pointer
	
	m_Cam.Xpos = 0.0f; //initialize camera settings
	m_Cam.Ypos = 0.0f;
	m_Cam.Zoom = 10.0f;
	m_Cam.Xrot = 310.0f;
	m_Cam.Yrot = 220.0f;
	m_Cam.Persp = 30.0f;
	m_Cam.TargetX = 0.0f;
	m_Cam.TargetY = 0.0f;
	m_Cam.TargetZ = 0.0f;
	
	m_TCam = m_Cam;

	//default view settings:
	bDrawBounds = false; //draw a generic bounding box
	bDrawAxes = true; //draw axes
	bDrawFloor = true; //Draw a tiled floor
	
	//IMPORTANT://
	//Set bTimer to true to lock a framerate and enable smooth camera movement
	//Set bTimer to false to Redraw only when the user changes the view.
	bTimer = true; 

}

//***********************************************************************************
//Destructor
//***********************************************************************************
COpenGL::~COpenGL(void)
{
}

//***********************************************************************************
//Initializes all the requisite OpenGL stuff (called once)
//***********************************************************************************
void COpenGL::GLInit(void) 
{

	static PIXELFORMATDESCRIPTOR pfd =
	{ 
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA,    // support OpenGL 
		24,                    // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		32,                    // 32-bit z-buffer (depth)
		0,                     // no stencil buffer 
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
    }; 

	// Get device context only once
	hdc = GetDC()->m_hDC;
	ASSERT(hdc);

	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	// Set color to use when clearing the background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	//Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_NORMALIZE);

	GLSetLighting(); //Set the global lighting

}

//***********************************************************************************
//Syncs the pointer to the view. This should be called once when the OpenGL window is created
//***********************************************************************************
void COpenGL::Sync(COpenGL_DemoView* p_View) //TODO: change name to your view class!!
{
	mp_View = p_View;
}

//***********************************************************************************
//Sets the Perspective (called whenever the perspective changes)
//***********************************************************************************
void COpenGL::GLSetPersp() 
{
	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity();	
	
	float aspect = (float)m_WindowRect.right / (float)m_WindowRect.bottom;
	gluPerspective(m_Cam.Persp, aspect, 0.01f, 2000.0f);

	glMatrixMode(GL_MODELVIEW); //Back to model view
}

//***********************************************************************************
//Sets up the lights and materials for rendering. Called every frame.
//***********************************************************************************
void COpenGL::GLSetLighting()
{
	glShadeModel(GL_SMOOTH); //smooth surfaces
	glEnable(GL_LIGHTING); //global lighting

	//LIGHT 0:
	glEnable(GL_LIGHT0);
	// Create light components for GL_LIGHT0
	float ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuseLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float specularLight0[] = { 0.5f, 0.5f, 0.0, 1.0f };
	float position0[] = { -5.0f, 5.0f, 20.0f, 1.0f };	
	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	//LIGHT 1:
	glEnable(GL_LIGHT1);
	// Create light components for GL_LIGHT1
	float diffuseLight1[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float specularLight1[] = { 0.3f, 0.1f, 0.1f, 1.0f };
	float position1[] = { 20.0f, 5.0f, 10.0f, 1.0f };	
	// Assign created components to GL_LIGHT1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);

	//LIGHT 2:
	glEnable(GL_LIGHT2);
	// Create light components for GL_LIGHT2
	float diffuseLight2[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float specularLight2[] = { 0.3f, 0.1f, 0.3f, 1.0f };
	float position2[] = { 10.0f, -10.0f, -10.0f, 1.0f };	
	// Assign created components to GL_LIGHT2
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight2);
	glLightfv(GL_LIGHT2, GL_POSITION, position2);

	//Global scene lighing setup
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default, but verify
	
	GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};  //Specular (highlight)
	GLfloat mat_shininess[] = {30};						//Shininess (size of highlight)
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); //Use ambient and diffuse
	glEnable(GL_COLOR_MATERIAL); //enable color tracking
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); //for accurate light reflections, mu
}

//***********************************************************************************
//The main Geometry drawing function!! This is where anything you want drawn will be placed.
//***********************************************************************************

void COpenGL::GLDrawScene()
{
	if (bDrawAxes)
		GLDrawAxes(false);

	if (bDrawBounds)
		GLDrawBounds();

	if (bDrawFloor)
		GLDrawFloor();

	//TODO: Add your ow drawing code here, or reference drawing code through your view class:
	//	mp_View->MyObject.GLDraw(); //(references fictional class)


}

//***********************************************************************************
//Draws a tiled floor
//***********************************************************************************
void COpenGL::GLDrawFloor()
{
	CGLUtils::glNormal(CVec(0,0,1));
	for (int i= -8; i<=8; i++) {
		for (int j= -8; j<=8; j++) {
			if ((i+j)%2==0) {
				glColor3d(0.5,0.5,0.7);
			} else {
				glColor3d(0.9,0.9,0.85);
			}
			glBegin(GL_POLYGON);
			CGLUtils::glVertex(CVec(i,j,-0.002));
			CGLUtils::glVertex(CVec(i+1,j,-0.002));
			CGLUtils::glVertex(CVec(i+1,j+1,-0.002));
			CGLUtils::glVertex(CVec(i,j+1,-0.002));
			glEnd();
		}
	}
}

//***********************************************************************************
//Draws a generic bounding box
//***********************************************************************************
void COpenGL::GLDrawBounds()
{

	CVec Dim = CVec(2,2,2); //TODO: set the bounding box size here.

	float i = (float)Dim.x;
	float j = (float)Dim.y;
	float k = (float)Dim.z;

	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(0,0,0); glVertex3f(i,0,0);
	glVertex3f(i,0,0); glVertex3f(i,j,0);
	glVertex3f(i,j,0); glVertex3f(0,j,0);
	glVertex3f(0,j,0); glVertex3d(0,0,0);
	glVertex3d(0,0,0); glVertex3d(0,0,k);
	glVertex3d(0,0,k); glVertex3d(i,0,k);
	glVertex3d(i,0,0); glVertex3d(i,0,k);
	glVertex3d(i,0,k); glVertex3d(i,j,k);
	glVertex3d(i,j,0); glVertex3d(i,j,k);
	glVertex3d(i,j,k); glVertex3d(0,j,k);
	glVertex3d(0,j,0); glVertex3d(0,j,k);
	glVertex3d(0,j,k); glVertex3d(0,0,k);
	glEnd();
}

//***********************************************************************************
//Draws Unit axes. Line = true draws line style axes, Line = false draws with 3D shapes.
//***********************************************************************************
void COpenGL::GLDrawAxes(bool Line) 
{
	if (Line == true){ //Draw Line style
		glBegin(GL_LINES);
		glColor3f(0,1,1); //Axes color
		glVertex3f(0,0,0); glVertex3f(1,0,0);
		glVertex3f(0,0,0); glVertex3f(0,1,0);
		glVertex3f(0,0,0); glVertex3f(0,0,1);
		glEnd();
	}
	else { //Draw Shape style
		float AxisScale = 1; //length of axes
		float AxisRad = 0.02f; //radius of axis lines

		glColor3d(0.7,0.3,0.3); //axis color
		CGLUtils::DrawSphere(AxisScale*AxisRad*2);
		CGLUtils::DrawCylinder(CVec(0,0,AxisScale),CVec(0,0,0),  AxisScale*AxisRad);
		CGLUtils::DrawCone(CVec(0,0,AxisScale+8*AxisRad), CVec(0,0,AxisScale), 3*AxisRad);
		CGLUtils::DrawCylinder(CVec(0,0,0), CVec(0,AxisScale,0), AxisScale*AxisRad);
		CGLUtils::DrawCone(CVec(0,AxisScale+8*AxisRad,0), CVec(0,AxisScale,0), 3*AxisRad);

		CGLUtils::DrawCylinder(CVec(0,0,0), CVec(AxisScale,0,0), AxisScale*AxisRad);
		CGLUtils::DrawCone(CVec(AxisScale+8*AxisRad,0,0), CVec(AxisScale, 0,0), 3*AxisRad);
	}
}

//Event Handlers:

//***********************************************************************************
//Passes Windows paint message to right place
//***********************************************************************************
void COpenGL::OnPaint()
{
	GLRedraw();
	ValidateRect(NULL); //Disable default painting to lock a framerate...
}

//***********************************************************************************
//Initializes OpenGL when window is created.
//***********************************************************************************
int COpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GLInit(); //initialize OpenGL

	if (bTimer)
		m_GLTimer = SetTimer(1, 30, 0); //initialize  animation timer at 30ms/frame (~33fps)

	return 0;
}

//***********************************************************************************
//IF bTimer is set to true, this function redraws the screen on a regular basis
//***********************************************************************************
void COpenGL::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1: //framerate update
		{
			GLRedraw(); //Draw the scene!

			//Update Camera position as function of target (Animate the camera)
			m_Cam.Xpos += (float)0.5*(m_TCam.Xpos-m_Cam.Xpos);
			m_Cam.Ypos += (float)0.5*(m_TCam.Ypos-m_Cam.Ypos);
			m_Cam.Zoom += (float)0.3*(m_TCam.Zoom-m_Cam.Zoom);
			m_Cam.Xrot += (float)0.3*(m_TCam.Xrot-m_Cam.Xrot);
			m_Cam.Yrot += (float)0.3*(m_TCam.Yrot-m_Cam.Yrot);
			m_Cam.Persp += (float)0.5*(m_TCam.Persp-m_Cam.Persp); 
			break;
		}
	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

//***********************************************************************************
//Keeps the OpenGL window filling the main window.
//***********************************************************************************
void COpenGL::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <=0 || nType == SIZE_MINIMIZED)
		return;

	//Map the OpenGL coordinates
	glViewport(0, 0, cx, cy);
	GetClientRect(m_WindowRect); //Get window size
	GLSetPersp(); //set current view perspective
}

//***********************************************************************************
//Handles the camera movement with the mous
//***********************************************************************************
void COpenGL::OnMouseMove(UINT nFlags, CPoint point)
{
	int diffX = (int)(point.x - m_fLastX); //difference in mouse coordinates
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX  = (float)point.x; //saves this mouse coordinate for next time this functionis called
	m_fLastY  = (float)point.y;
	
	CAMERA* pCamera = &m_Cam; //Update the actual camera position...
	if(bTimer) //unless we want to animate the camera
		pCamera = &m_TCam;
	
	if (nFlags & MK_MBUTTON) //If the middle mouse button is down...
	{
		if (nFlags & MK_CONTROL) //...and control is pressed, do panning
		{
			pCamera->Xpos += (float)0.01f * diffX;
			pCamera->Ypos -= (float)0.01f * diffY;
		}
		
		else if (nFlags & MK_SHIFT) //..and shift is down, change perspective
		{
			pCamera->Persp += (float)0.1f * diffY;
			if (pCamera->Persp < 5)  //reasonable limits on perspective
				pCamera->Persp = 5;
			if (pCamera->Persp > 90)
				pCamera->Persp = 90;
		}

		else //...or if no other buttons are pressed, do rotate
		{
			pCamera->Xrot += (float)0.5f * diffY;
			
			if (pCamera->Xrot > 360) //Keep in reasonable range...
				pCamera->Xrot -= 360;
			if (pCamera->Xrot < 0) //Keep in reasonable range...
				pCamera->Xrot += 360;

			pCamera->Yrot += (float)0.5f * diffX;

			if (pCamera->Yrot > 360) //Keep in reasonable range...
				pCamera->Yrot -= 360;
			if (pCamera->Yrot < 0) //Keep in reasonable range...
				pCamera->Yrot += 360;
		}
		if (!bTimer) //if the timer is not running, redraw right now.
			GLRedraw();
	}

	CWnd::OnMouseMove(nFlags, point);
}

//***********************************************************************************
//Handles the mouse wheel for zooming
//***********************************************************************************
BOOL COpenGL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CAMERA* pCamera = &m_Cam; //Update the actual camera position...
	if(bTimer) //unless we want to animate the camera
		pCamera = &m_TCam;

	if (zDelta > 0) //zoom in or out
		pCamera->Zoom *= (float)1.05;
	if (zDelta < 0)
		pCamera->Zoom *= (float).95;

	if (!bTimer) //if the timer is not running, redraw right now.
		GLRedraw();
	
	return true;
}

//***********************************************************************************
//Redraws the OpenGL window. Call this whenever you want to update the view.
//***********************************************************************************
void COpenGL::GLRedraw(void)
{
	wglMakeCurrent(hdc, hrc); //This makes the this openGL window current if there is more than one.
	// new frame:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer bits
			
	//Bonuses: Disable for greates rendering speed
	GLSetPersp(); //Enable to have dynamic control over the perspective
	GLSetLighting(); //Enable to have specular highlight in accurate place
	//End Bonuses

	//Start drawing process
	glLoadIdentity();

	// set camera view
	glTranslatef(0.0f, 0.0f, -m_Cam.Zoom);
	glTranslatef(m_Cam.Xpos, m_Cam.Ypos, 0.0f);
	glRotatef(m_Cam.Xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(m_Cam.Yrot, 0.0f, 0.0f, 1.0f);
	glTranslatef(m_Cam.TargetX, m_Cam.TargetY, m_Cam.TargetZ);

	GLDrawScene(); // Draw OpenGL scene
	SwapBuffers(hdc); // Swap buffers

}



