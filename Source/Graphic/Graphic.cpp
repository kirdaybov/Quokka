#include "Graphic.h"
#include "Application/Application.h"
#include "RenderEvent.h"
#include "Camera/CameraEvent.h"

namespace quokka
{

	void OpenGLGraphic::InitForWindows(HWND hWnd)
	{
		// Device context
		hDC = GetDC(hWnd);

		// Set pixel format
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof( pfd );
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		int iFormat = ChoosePixelFormat( hDC, &pfd );
		SetPixelFormat( hDC, iFormat, &pfd );

		// Create render context
		hRC = wglCreateContext(hDC);

		// TODO: maybe put it in a different method?
		wglMakeCurrent(hDC, hRC);
	}

	int OpenGLGraphic::AddObjectToRender(Event* A_RenderEvent)
	{
		RenderEvent* re = dynamic_cast<RenderEvent*>(A_RenderEvent);
		ObjectsToRender.push_back(re->GetRenderObject());
		return 0;
	}

	int OpenGLGraphic::UpdateCamera(Event* A_CameraEvent)
	{
		CameraEvent* ce = dynamic_cast<CameraEvent*>(A_CameraEvent);
		CameraPosition = ce->GetCameraPosition();
		return 0;
	}

	void OpenGLGraphic::Draw(float time)
	{
		glClearColor( 0.5f, 0.5f, 0.5f, 0.5f );
		glClear( GL_COLOR_BUFFER_BIT );

		glPushMatrix();
		glRotatef( 0.0f, 0.0f, 0.0f, 1.0f );
    
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 1.f, 1.f);
    glVertex2f(-1.f, -1.f);

    glColor3f(1.f, 0.f, 0.f);
    glVertex2f(-0.9f, -1.f);

    glColor3f(0.f, 0.f, 1.f);
    glVertex2f(-1.f, -0.9f);
    glEnd();

		glBegin( GL_QUADS );			
		
		for(RenderObject Object : ObjectsToRender)
		{
			DrawObject(Object);
		}

    //glColor3f(1, 1, 1); glVertex2f(-0.5f, -0.5f);
    //glColor3f(1, 1, 0); glVertex2f(-0.5f,  0.5f);
    //glColor3f(1, 1, 0); glVertex2f( 0.5f, 0.5f);
    //glColor3f(1, 1, 0); glVertex2f( 0.5f, -0.5f);

		glEnd();
		glPopMatrix();

		SwapBuffers(hDC);

		ObjectsToRender.clear();
	}

	void OpenGLGraphic::Shutdown()
	{
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);
	}

	void OpenGLGraphic::DrawObject(RenderObject Object)
	{
		float ScreenRatio = GApp()->Height()/GApp()->Width();
    Vector v[4] =
    {
      Object.Rotation.RotateVector(Vector( - Object.Size.x / 2, - Object.Size.y / 2)),
      Object.Rotation.RotateVector(Vector( - Object.Size.x / 2, + Object.Size.y / 2)),
      Object.Rotation.RotateVector(Vector( + Object.Size.x / 2, + Object.Size.y / 2)),
      Object.Rotation.RotateVector(Vector( + Object.Size.x / 2, - Object.Size.y / 2))
    };
    for (int i = 0; i < 4; i++)
    {
      glColor3f(Object.Color.r, Object.Color.g, Object.Color.b); 
      glVertex2f(Scale*ScreenRatio*(Object.Position.x + v[i].x - CameraPosition.x), Scale*(Object.Position.y + v[i].y - CameraPosition.y));
    }
      
	}
}