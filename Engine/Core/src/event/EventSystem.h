#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "IEvent.h"
#include "IEventListener.h"
#include <map>
#include <vector>
#include <memory>

namespace Core {

	class EventSystem {
	public:
		EventSystem();
		~EventSystem();

		template<typename T>
		void registerListener(EventType type, T& listener);
		void dispatchEvent(IEvent& event);

	private:
		std::map<EventType, std::vector<std::shared_ptr<IEventListener>>> listeners;
	};

	template<typename T>
	void EventSystem::registerListener(EventType type, T& listener) {
		listeners[type].push_back(std::make_shared<T>(listener));
	}
}
#endif