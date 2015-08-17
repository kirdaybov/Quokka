#pragma once
#include "Common/Timer.h"
#include "Events/EventManager.h"
#include "Physics/Physics.h"

#include <vector>
#include <memory>
#include "Actor/Actor.h"
#include "Profiler/Profiler.h"

namespace quokka
{
  

  class Game : public Singletone<Game>
  {
		friend class ActorFactory;
  public:
    Game()
      :bStop(false)
    {
      Instance = this;
      GEventManager()->AddListener(EventType::MOUSE_CLICK, new Delegate<int, Event*>(this, &Game::SpawnPerson));    
    }

		void Init();

    Physics* GetPhysics() { return Physics; }

    int SpawnPerson(Event* A_MouseClickEvent);
		
    void Tick()
    {
      GProfiler()->Start("Game->Physics");
      Physics->Tick();
      GProfiler()->Stop("Game->Physics");

      GProfiler()->Start("Game->Actorstick");
      for (int i = 0; i < Actors.size(); i++)
			{
				Actors[i]->Tick();
			}
      GProfiler()->Stop("Game->Actorstick");

      for (int i = 0; i < Actors.size(); i++)
      {        
        if (Actors[i]->NeedsDelete())
        { 
          Actors.erase(Actors.begin() + i);
          i--;
        }          
      }
    }

		void Shutdown()
		{
      Actors.clear();
		}

    std::vector<std::shared_ptr<Actor>>* GetAllActors()
    {
      return &Actors;
    }

    template<class T>
    std::vector<std::weak_ptr<Actor>> GetActorsByClass()
    {
      std::vector<std::weak_ptr<Actor>> res;
      for (auto Actor : Actors)
      {
        T* Candidate = dynamic_cast<T*>(Actor.get());
        if (Candidate)
          res.push_back(Actor);
      }

      return res;
    }

    ActorFactory* Factory = nullptr;    
  private:

    Vector GetCameraPosition()
    {
      if (Hero)
        return Hero->GetWorldPosition();
      else
        return Vector();
    }

    bool bStop;

    Actor* Hero = nullptr;	
	
	  Physics* Physics;	
		std::vector<std::shared_ptr<Actor>> Actors;    
  };


  Game* GGame();
}