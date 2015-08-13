#pragma once

#include <windows.h>
#include <vector>
#include <GL/GL.h>

//#include "Quokka.h"
#include "Common/Singletone.h"
#include "RenderObject.h"
#include "Events/EventManager.h"


namespace quokka
{

	class Graphic
	{
	public:		
		virtual void Draw(float time) = 0;
		virtual void Shutdown() = 0;
	};

	class OpenGLGraphic : public Graphic
	{
	public:
		OpenGLGraphic() : CameraPosition(0.f)
		{
			GEventManager()->AddListener(EventType::RENDER, new Delegate<int, Event*>(this, &OpenGLGraphic::AddObjectToRender));
			GEventManager()->AddListener(EventType::CAMERA_UPDATE, new Delegate<int, Event*>(this, &OpenGLGraphic::UpdateCamera));
		}
	
		void InitForWindows(HWND hWnd);
		int AddObjectToRender(Event* A_RenderEvent);
		int UpdateCamera(Event* A_CameraEvent);

		virtual void Draw(float time) override;
		virtual void Shutdown() override;

	private:
		void DrawObject(RenderObject Object);

		HGLRC hRC;
		HWND hWnd;
		HDC hDC;

		Vector CameraPosition;
		std::vector<RenderObject> ObjectsToRender;

    float Scale = 0.3f;
	};

}