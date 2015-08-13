#pragma once 

#include "Actor/Actor.h"
#include "Actor/ActorComponent.h"
#include "Movement/MovementComponent.h"
#include "Movement/MovementInputEvent.h"
#include "Common/Timer.h"

namespace quokka
{
	class ControllerComponent : public ActorComponent
	{
	public:
		ControllerComponent() 
			: ActorComponent()
		{
			GEventManager()->AddListener(EventType::MOVEMENT_INPUT, new Delegate<int, Event*>(this, &ControllerComponent::Move));
		}

		int Move(Event* A_MovementEvent)
		{
			bWasMovement = true;

			MovementInputEvent* mie = dynamic_cast<MovementInputEvent*>(A_MovementEvent);
			Movement = mie->Movement;			
			return 0;
		}

		void Tick() override
		{	
			if(bWasMovement)
			{
				MovementComponent* MovementComp = GetOwner()->GetComponent<MovementComponent>();
				if(MovementComp)
				{
          MovementComp->Move(Movement);
				}
				else
					GetOwner()->SetWorldPosition(GetOwner()->GetWorldPosition() + Movement);
			}				
			
			bWasMovement = false;
		}

		bool IsMovementAllowed()
		{

		}

	private:
		bool bWasMovement;
		Vector Movement;
	};
}