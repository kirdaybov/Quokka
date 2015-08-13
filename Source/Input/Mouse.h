#pragma once

#include <Windows.h>

namespace quokka
{

  class Mouse
  {
  public:
    Mouse();
    POINT GetPosition()
    {
      POINT p;
      if (GetCursorPos(&p))
        ScreenToClient(hWnd, &p);

      return p;
    }
  private:
    HWND hWnd;
  };
}