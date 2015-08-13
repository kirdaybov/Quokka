#pragma once

#include "Actor/ActorComponent.h"
#include "CameraEvent.h"

namespace quokka
{
	class CameraComponent : public ActorComponent
	{
	public:
		void Tick() override
		{
			Position = GetOwner()->GetWorldPosition();
			GEventManager()->PostEvent(new CameraEvent(Position));
		}
	private:
		Vector Position;
	};
}