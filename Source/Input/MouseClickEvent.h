#pragma once 

#include "Events/Event.h"
#include "Common/Vector.h"

namespace quokka
{
  class MouseClickEvent : public Event
  {
  public:
    MouseClickEvent(Vector MousePosition, bool bRight)
      : Event(EventType::MOUSE_CLICK)
      , MousePosition(MousePosition)
      , bRight(bRight)
    {}
    Vector MousePosition;
    bool bRight = false;
  };
}