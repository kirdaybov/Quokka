#pragma once 

#include "Actor/Actor.h"
#include "Actor/ActorComponent.h"
#include "MovementInputEvent.h"
#include "Common/Timer.h"

namespace quokka
{
	class MovementComponent : public ActorComponent
	{
	public:
		MovementComponent(float A_Speed) 
			: ActorComponent()
			, Speed(A_Speed)
		{}

		void Move(Vector Movement)
		{
      bWasMovement = true;      			
      
      //TODO: doesn't use inertia			
			//this->Movement = Movement.Normalize()*Speed*Timer()->GetFrameTime();			

      float DesiredRotation = QRotation::FromDirection(Movement).x;
      float CurrentRotation = GetOwner()->GetRotation().x;
      
      QRotation AddRotation;

      if (fabsf(DesiredRotation - CurrentRotation) > M_PI) DesiredRotation += DesiredRotation > CurrentRotation ? -2 * M_PI : 2 * M_PI;
      if (fabsf(DesiredRotation - CurrentRotation) < RotationSpeed*Timer()->GetFrameTime())
        AddRotation.x = DesiredRotation - CurrentRotation;
      else
        AddRotation.x = DesiredRotation > CurrentRotation ? RotationSpeed*Timer()->GetFrameTime() : -RotationSpeed*Timer()->GetFrameTime();
            
      GetOwner()->AddRotation(AddRotation);

      this->Movement = GetOwner()->GetRotation().RotateVector(Vector(1.f, 0.f))*Speed*Timer()->GetFrameTime();
		}

		void Tick() override
		{	
			if(bWasMovement)
			{
				PhysicsComponent* PhComp = GetOwner()->GetComponent<PhysicsComponent>();
				if(PhComp)
				{
					Vector NewPosition = PhComp->GetPhysObject()->Position + Vector(Movement.x, 0.f);
					GGame()->GetPhysics()->MoveObject(PhComp->GetPhysObject(), NewPosition);
					NewPosition = PhComp->GetPhysObject()->Position + Vector(0.f, Movement.y);
					GGame()->GetPhysics()->MoveObject(PhComp->GetPhysObject(), NewPosition);
				}
				else
					GetOwner()->SetWorldPosition(GetOwner()->GetWorldPosition() + Movement);
			}				
			
			bWasMovement = false;
		}

	private:
		bool bWasMovement;
		Vector Movement;
		float Speed;
    float RotationSpeed = M_PI;
	};
}