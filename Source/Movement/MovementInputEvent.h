#pragma once 

#include "Events/Event.h"
#include "Common/Vector.h"

namespace quokka
{
	class MovementInputEvent : public Event
	{
	public:
		MovementInputEvent(float A_Right, float A_Forward) 
			: Event(EventType::MOVEMENT_INPUT)
			,Movement(A_Right, A_Forward)
		{}
		Vector Movement;
	};
}