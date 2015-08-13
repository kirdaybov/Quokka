#pragma once

namespace quokka
{
	enum class EventType
	{
		RENDER,
		MOVEMENT_INPUT,
		CAMERA_UPDATE,
    MOUSE_CLICK
	};

	class Event
	{
	public:
		Event(EventType A_Type) 
			:Type(A_Type)
		{}
		virtual ~Event() {}
		EventType GetEventType() { return Type; }

	protected:
		EventType Type;
	};
}