#pragma once 

#include "Events/Event.h"
#include "Common/Vector.h"

namespace quokka
{
	class CameraEvent : public Event
	{
	public:
		CameraEvent(Vector A_CameraPosition) : Event(EventType::CAMERA_UPDATE)
			,CameraPosition(A_CameraPosition)
		{}
		Vector GetCameraPosition() { return CameraPosition; }
	private:
		Vector CameraPosition;
	};
}