#pragma once 

#include "Events/Event.h"
#include "RenderObject.h"

namespace quokka
{
	class RenderEvent : public Event
	{
	public:
		RenderEvent(RenderObject A_RenderObject) 
			: Event(EventType::RENDER)
			,RenderObject(A_RenderObject) 
		{}
		RenderObject& GetRenderObject() { return RenderObject; }
	private:
		RenderObject RenderObject;
	};
}