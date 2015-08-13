#pragma once
#include <map>

namespace quokka
{
	
	struct KeyboardKey
	{
		bool bDown;
		bool bPressed;
		bool bReleased;

		KeyboardKey()
			:bDown(false)
			,bPressed(false)
			,bReleased(false)
		{}
	};

	class Keyboard
	{
	public:
		Keyboard()
		{
			for (int i = 0; i < KEY_BUFFER_SIZE; i++) Keys[i] = KeyboardKey();
		}

		void OnKeyPressed(unsigned int KeyCode)
		{			
			Keys[KeyCode].bPressed = true;			
		}
		void OnKeyReleased(unsigned int KeyCode)
		{
			Keys[KeyCode].bReleased = true;
		}

		void Tick()
		{
			for(int i = 0; i < KEY_BUFFER_SIZE; i++)
			{
				if(Keys[i].bPressed) Keys[i].bDown = true;
				if(Keys[i].bReleased) Keys[i].bDown = false;

				Keys[i].bPressed  = false;
				Keys[i].bReleased = false;
			}
		}
		bool IsPressed(int KeyCode) { return Keys[KeyCode].bPressed; }
		bool IsReleased(int KeyCode) { return Keys[KeyCode].bReleased; }
		bool IsDown(int KeyCode) { return Keys[KeyCode].bDown; }

	private:
		static const int KEY_BUFFER_SIZE = 256;
		KeyboardKey Keys[KEY_BUFFER_SIZE]; 
	};
}