#include "Game.h"
#include "Input/MouseClickEvent.h"
#include "ActorFactory.h"

namespace quokka
{
	void Game::Init()
	{
    Physics = new quokka::QPhysics();

    PlayerStates.push_back(new QPlayerState());

		Factory = new ActorFactory(this);
		//Factory->CreateWorld(30);
		//Hero = Factory->CreateHero();
    //for (int i = 0; i < 100; i++)
    //  Factory->CreateEnemy(Vector(rand() % 100, rand() % 100)/100.f + Vector(1.f));		
	}

  int Game::SpawnPerson(Event* A_MouseClickEvent)
  {
    MouseClickEvent* mce = dynamic_cast<MouseClickEvent*>(A_MouseClickEvent);
    Vector SpawnPosition = GetCameraPosition();
    // BAD CODE!
    mce->MousePosition.x = (mce->MousePosition.x / 1024 - 0.5f)*1024/768/0.3f*2;
    mce->MousePosition.y = -(mce->MousePosition.y / 768 - 0.5f)         /0.3f*2;
    
    if (mce->bRight)
      std::weak_ptr<Actor>(Factory->CreateFood(SpawnPosition + mce->MousePosition));
    else
      Factory->CreateGenderedPerson(SpawnPosition + mce->MousePosition, rand() % 2 == 0);

    return 0;
  }

  Game* GGame() { return Game::GetInstance(); }
  Game* Game::Instance = nullptr;
}