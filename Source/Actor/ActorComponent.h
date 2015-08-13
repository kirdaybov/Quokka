#pragma once

namespace quokka
{
	class ActorComponent
	{
		friend class Actor;
	public:
		Actor* GetOwner() { return Owner; }
    virtual ~ActorComponent() {}

		virtual void Tick() = 0;
	protected:
		virtual void SetOwner(Actor* Owner) { this->Owner = Owner; }
	private:
		Actor* Owner;
	};
}