#pragma once

#include "Application/Application.h"
#include "Actor/ActorComponent.h"

#include <iostream>

namespace quokka
{
	class PhysicsComponent : public ActorComponent
	{
	public:
		PhysicsComponent(Vector A_Size = Vector(0.1f, 0.1f)) : ActorComponent()
			,Size(A_Size)
		{}

    virtual ~PhysicsComponent()
    {
      GGame()->GetPhysics()->DeletePhysObject(PhysObject);
    }

		void SetOwner(Actor* Owner) override
		{
			ActorComponent::SetOwner(Owner);
			PhysObject = GGame()->GetPhysics()->CreateSpherePhysObject(Owner->GetWorldPosition(), Size.x);
		}

		void Tick() override 
		{
			GetOwner()->SetWorldPosition(PhysObject->Position);
		};
		
		Vector Size;

		PhysObject* GetPhysObject() { return PhysObject; };
	private:
		PhysObject* PhysObject;
	};
}