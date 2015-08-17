#pragma once 
#include "Game.h"
#include "Graphic/RenderComponent.h"
#include "Movement/ControllerComponent.h"
#include "Movement/MovementComponent.h"
#include "Camera/CameraComponent.h"
#include "AI/AIComponent.h"

namespace quokka
{
  class Food : public Actor
  {
  };

	class ActorFactory
	{
	public:
		ActorFactory(Game* A_Game) : Game(A_Game){}

		Actor* CreateHero() 
		{
			Actor* Hero = new Actor();
			Hero->SetWorldPosition(0.f);
			//Hero->AddComponent(new RenderableComponent(Color(0.f, 1.f, 0.f)));
			Hero->AddComponent(new MovementComponent(1.0f));
      Hero->AddComponent(new ControllerComponent());
			//Hero->AddComponent(new PhysicsComponent(Vector(0.1f, 0.1f)));
			Hero->AddComponent(new CameraComponent());

			Game->Actors.push_back(std::shared_ptr<Actor>(Hero));

      return Hero;
		}

		void CreateEnemy(Vector WorldPosition)
		{
			Actor* Enemy = new Actor();
			Enemy->SetWorldPosition(WorldPosition);
      Enemy->AddComponent(new AIComponent(rand() % 2));
      bool bFemale = Enemy->GetComponent<AIComponent>()->IsFemale();
			
      Enemy->AddComponent(new RenderableComponent(bFemale ? Color(1.f, .6f, .6f) : Color(.6f, .6f, 1.f)));
      Enemy->AddComponent(new MovementComponent(1.0f));      
			Enemy->AddComponent(new PhysicsComponent(Vector(0.1f, 0.1f)));

      Game->Actors.push_back(std::shared_ptr<Actor>(Enemy));
		}

    std::shared_ptr<Actor> CreateFood(Vector WorldPosition)
    {
      Actor* lFood = new Food();
      lFood->SetWorldPosition(WorldPosition);
      lFood->AddComponent(new RenderableComponent(Color(0.f, 1.f, 0.f)));
      lFood->AddComponent(new PhysicsComponent(Vector(0.03f, 0.03f)));
      std::shared_ptr<Actor> pFood(lFood);
      Game->Actors.push_back(pFood);
      return pFood;
    }

    void CreateGenderedPerson(Vector WorldPosition, bool bFemale)
    {
      Actor* Enemy = new Actor();
      Enemy->SetWorldPosition(WorldPosition);
      Enemy->AddComponent(new AIComponent(bFemale));
      Enemy->AddComponent(new RenderableComponent(bFemale ? Color(1.f, .6f, .6f) : Color(.6f, .6f, 1.f)));
      Enemy->AddComponent(new MovementComponent(1.0f));
      Enemy->AddComponent(new PhysicsComponent(Vector(0.1f, 0.1f)));

      Game->Actors.push_back(std::shared_ptr<Actor>(Enemy));
    }

		void CreateWall(Vector WorldPosition, Vector Size)
		{
			Actor* Wall = new Actor();
			Wall->SetWorldPosition(WorldPosition);
			Wall->AddComponent(new RenderableComponent(Color(0.f, 0.f, 0.f)));
			Wall->AddComponent(new PhysicsComponent(Size));

      Game->Actors.push_back(std::shared_ptr<Actor>(Wall));
		}

		void CreateWorld(int Size)
		{
			for(int i = 0; i < Size; i++)
			{
				bool bHorizontal = rand() % 2 == 0;
				int Unit = rand() % 2 == 0 ? 1 : -1;
				float x = (rand() % 10 + 2)*0.2f * Unit;
				Unit = rand() % 2 == 0 ? 1 : -1;
				float y = (rand() % 10 + 2)*0.2f * Unit;
				float Width =		bHorizontal ? (rand() % 20)*0.1f : 0.1f;
				float Height = !bHorizontal ? (rand() % 20)*0.1f : 0.1f;
				CreateWall(Vector(x, y), Vector(Width, Height));
			}
		}
	private:
		Game* Game;
	};
}