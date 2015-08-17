#include "Game/ActorFactory.h"
#include "AIComponent.h"


namespace quokka
{
  void AIComponent::DoStuff()
  {    
  }

  void AIComponent::StartMove()
  {
    State = AIState::Moving;
    Target = GetOwner()->GetWorldPosition() + Vector(rand() % 11 - 5, rand() % 11 - 5) / 5.f;
    for (auto iFood : GGame()->GetActorsByClass<Food>())
    {
      auto pFood = iFood.lock();
      float Distance = (pFood->GetWorldPosition() - GetOwner()->GetWorldPosition()).Size();
      if (Distance < 0.2f)
      {
        Age += 1.f;
        pFood->MarkForDelete();
      }        
      else
      if (Distance < 1.5f)
        Target = pFood->GetWorldPosition();
    }
  }
}