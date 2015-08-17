#pragma once
#include <vector>

#include "Common/Vector.h"
#include "ActorComponent.h"

namespace quokka
{  
  struct QTransform
  {
    Vector Position;
    QRotation Rotation;
  };

	class Actor
	{
	public:
		virtual ~Actor()
		{
			for(ActorComponent* Component : Components)
			{
				delete Component;
				Component = nullptr;
			}
		}

		void Tick()
		{
			for (ActorComponent* Component : Components)
				Component->Tick();
		}

		void AddComponent(ActorComponent* Component)
		{
			if(Component->Owner)			
				throw 0;
			Component->SetOwner(this);
			Components.push_back(Component);			
		}

		template<typename T>
		T* GetComponent()
		{
			for(ActorComponent* Component : Components)
			{
				T* T_Component = dynamic_cast<T*>(Component);
				if (T_Component)
					return T_Component;
			}

			return nullptr;
		}

    void MarkForDelete() { bMarkedForDelete = true; }
    bool NeedsDelete() { return bMarkedForDelete; }

    void AddRotation(QRotation Rotation) { Transform.Rotation = Transform.Rotation + Rotation; Transform.Rotation.Clamp(); }
    QRotation GetRotation() { return Transform.Rotation; }

		void SetWorldPosition(Vector Position) { Transform.Position = Position; }
		Vector GetWorldPosition() { return Transform.Position; }
	private:
    QTransform Transform;
		std::vector<ActorComponent*> Components;

    bool bMarkedForDelete = false;
	};
}