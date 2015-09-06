#pragma once

#include "Actor/Actor.h"
#include "Actor/ActorComponent.h"

#include "Physics/PhysicsComponent.h"

#include "Common/Color.h"
#include "RenderObject.h"
#include "RenderEvent.h"

namespace quokka
{ 

	class RenderableComponent : public ActorComponent
	{
	public:
		RenderableComponent(Color A_Color, std::string A_Texture = ""):ActorComponent()
      , Color(A_Color), Texture(A_Texture)
		{}
		virtual void Tick()
		{
			RenderObject ro;
			ro.Position = GetOwner()->GetWorldPosition();			
      ro.Rotation = GetOwner()->GetRotation();      
      ro.Texture = Texture;

			PhysicsComponent* PhComp = GetOwner()->GetComponent<PhysicsComponent>();
			if(PhComp)
				ro.Size = PhComp->Size;
			else
				ro.Size = Vector(0.1f, 0.1f);
			ro.Color = Color;

			GEventManager()->PostEvent(new RenderEvent(ro));
		}
	private:
		Color Color;
    std::string Texture;
	};
}