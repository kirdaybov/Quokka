#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Movement/MovementInputEvent.h"
#include "MouseClickEvent.h"

#include <iostream>

namespace quokka
{
	
	class Input
	{
	public:
		void Tick()
		{
			Keyboard.Tick();						
			
			float Right = 0.f;
			float Forward = 0.f;
			if(Keyboard.IsDown(VK_LEFT))	Right -= 1.0f;
			if(Keyboard.IsDown(VK_RIGHT)) Right += 1.0f;
			if(Keyboard.IsDown(VK_UP))		Forward += 1.0f;
			if(Keyboard.IsDown(VK_DOWN))	Forward -= 1.0f;
			GEventManager()->PostEvent(new MovementInputEvent(Right, Forward));
		}
		
		void Keyboard_OnKeyPressed(unsigned int KeyCode)
		{
			Keyboard.OnKeyPressed(KeyCode);
		}

		void Keyboard_OnKeyReleased(unsigned int KeyCode)
		{
			Keyboard.OnKeyReleased(KeyCode);
		}

    void Mouse_RightButtonDown()
    {
      POINT p = Mouse.GetPosition();
      GEventManager()->PostEvent(new MouseClickEvent(Vector(p.x, p.y), true));
    }

    void Mouse_LeftButtonDown()
    {
      POINT p = Mouse.GetPosition();
      GEventManager()->PostEvent(new MouseClickEvent(Vector(p.x, p.y), false));
    }

	private:
		Keyboard Keyboard;
    Mouse Mouse;
	};
}