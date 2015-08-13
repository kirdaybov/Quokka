#pragma once

#include "Common/Singletone.h"
#include "Common/Delegates.h"
#include "Event.h"

#include <map>
#include <vector>

namespace quokka
{
	class EventManager : public Singletone<EventManager>
	{
	public:
		void AddListener(EventType Type, Delegate<int, Event*>* Delegate)
		{
			Listeners[Type].push_back(Delegate);
		}

		void PostEvent(Event* Event)
		{
			Events.push_back(Event);
		}

		void Tick()
		{
			for (Event* L_Event : Events)
			{
				for(Delegate<int, Event*>* Delegate : Listeners[L_Event->GetEventType()])
					(*Delegate)(L_Event);
			}

			Events.clear();
		}
	private:
		std::vector<Event*> Events;
		std::map<EventType, std::vector<Delegate<int, Event*>*>> Listeners;
	};

	EventManager* EventManager::Instance = NULL;
	
	static EventManager* GEventManager() 
	{
		return EventManager::GetInstance();
	}
}