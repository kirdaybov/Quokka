#include "Graphic.h"
#include "Application/Application.h"
#include "RenderEvent.h"
#include "Camera/CameraEvent.h"

namespace quokka
{
  struct TGA_HEADER
  {
    char  identsize;          // size of ID field that follows 18 byte header (0 usually)
    char  colourmaptype;      // type of colour map 0=none, 1=has palette
    char  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    char colourmapstart;     // first colour map entry in palette
    char colourmaplength;    // number of colours in palette

    short  colourmapbits;      // number of bits per palette entry 15,16,24,32

    short xstart;             // image x origin
    short ystart;             // image y origin
    short width;              // image width in pixels
    short height;             // image height in pixels

    char  bits;               // image bits per pixel 8,16,24,32
    char  descriptor;         // image descriptor bits (vh flip bits)

    // pixel data follows header    
  };

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

    InitTexture();
	}

  void OpenGLGraphic::InitTexture()
  {
    std::string TextureFolder;
    LoadImage("Ship.tga");

    HANDLE hFind;
    WIN32_FIND_DATA data;

    hFind = FindFirstFile((GApp()->GetExePath() + "\\Textures/*.*").c_str(), &data);
    if (hFind != INVALID_HANDLE_VALUE) {
      do {
        //TODO: make a function
        {
          std::string filename = data.cFileName;
          if (filename.length() < 4) continue;
          std::string format = filename.substr(filename.length() - 4, 4);
          if (format.compare(".tga") == 0)
            LoadImage(filename);
        }
        printf("%s\n", data.cFileName);
      } while (FindNextFile(hFind, &data));
      FindClose(hFind);
    }

  }

  void OpenGLGraphic::LoadImage(std::string filename)
  {
    // Читаем заголовок TGA
    TGA_HEADER header;
        
    std::string path = GApp()->GetExePath();
    path.append("\\Textures\\"); 
    path.append(filename);

    FILE* file;
    fopen_s(&file, path.c_str(), "rb");
    fread(&header, sizeof(TGA_HEADER), 1, file);

    // Вынести
    int width = header.width;
    int height = header.height;

    BYTE* data = (BYTE*)malloc(width*height * 4);

    fread(data, width*height * 4, 1, file);
    fclose(file);

    BYTE* current = data;
    for (int i = 0; i < width*height * 4; i++)
    {
      if (i % 4 == 2)
      {
        BYTE* red = current - 2;
        BYTE* blue = current;
        BYTE buffer = *blue;
        memcpy(blue, red, 1);
        memcpy(red, &buffer, 1);
      }
      current++;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);

    filename.erase(filename.length() - 4, 4);    
    Textures[filename] = texture;
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		for(RenderObject Object : ObjectsToRender)
		{
			DrawObject(Object);
		}

    //glColor3f(1, 1, 1); glVertex2f(-0.5f, -0.5f);
    //glColor3f(1, 1, 0); glVertex2f(-0.5f,  0.5f);
    //glColor3f(1, 1, 0); glVertex2f( 0.5f, 0.5f);
    //glColor3f(1, 1, 0); glVertex2f( 0.5f, -0.5f);

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
    glBindTexture(GL_TEXTURE_2D, Textures[Object.Texture]);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBegin(GL_QUADS);

		float ScreenRatio = GApp()->Height()/GApp()->Width();
    Vector v[4] =
    {
      Object.Rotation.RotateVector(Vector( - Object.Size.x / 2, - Object.Size.y / 2)),
      Object.Rotation.RotateVector(Vector( - Object.Size.x / 2, + Object.Size.y / 2)),
      Object.Rotation.RotateVector(Vector( + Object.Size.x / 2, + Object.Size.y / 2)),
      Object.Rotation.RotateVector(Vector( + Object.Size.x / 2, - Object.Size.y / 2))
    };

    Vector t[4] = { Vector(0, 0), Vector(1, 0), Vector(1, 1), Vector(0, 1) };
    for (int i = 0; i < 4; i++)
    {
      //glColor3f(Object.Color.r, Object.Color.g, Object.Color.b); 
      glTexCoord2f(t[i].x, t[i].y);
      glVertex2f(Scale*ScreenRatio*(Object.Position.x + v[i].x - CameraPosition.x), Scale*(Object.Position.y + v[i].y - CameraPosition.y));
    }
      
    glEnd();
	}
}