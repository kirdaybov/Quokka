#pragma once 

#include "Actor/ActorComponent.h"
#include "Common/Timer.h"


namespace quokka
{
  enum class AIState
  {
    None = 0,
    Moving, 
    Idle,
    DoStuff
  };

  class AIComponent : public ActorComponent
  {
  public:
    AIComponent(bool bFemale = false) 
    :ActorComponent()
    , bFemale(bFemale)
    {}
    void Tick() override
    {
      switch (State)
      {
      case AIState::None: Think(); break;
      case AIState::Moving: Move(); break;
      case AIState::Idle: Wait(); break;
      case AIState::DoStuff: DoStuff(); break;
      }

      Age -= Timer()->GetFrameTime();
      if (Age < 0.f)
        GetOwner()->MarkForDelete();
    }

    void StartMove()
    {
      State = AIState::Moving;
      Target = GetOwner()->GetWorldPosition() + Vector(rand() % 11 - 5, rand() % 11 - 5)/5.f;
    }

    void Move()
    {
      Vector Direction = Target - GetOwner()->GetWorldPosition();

      if (Direction.Size() < 0.01f)
        return Think();

      MovementComponent* MovementComp = GetOwner()->GetComponent<MovementComponent>();
      if (MovementComp)
        MovementComp->Move(Direction);
    }

    void Wait()
    {
      State = AIState::Idle;
      Time -= Timer()->GetFrameTime();

      if (Time < 0)
      {
        Time = rand() % 3;
        Think();
      }
    }

    void DoStuff();

    void Think()
    {      
      int x = rand() % 100;
      if (x < 50) StartMove();
      else Wait();      
    }

    bool IsFemale() { return bFemale; }

  private:
    
    AIState State = AIState::None;
    Vector Target;
    float Time = 2.f;        

    bool bFemale = false;

    float Age = 5.f;
  };
}
