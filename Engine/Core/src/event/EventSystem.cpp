#include "EventSystem.h"
using namespace Core;


EventSystem::EventSystem() {}
EventSystem::~EventSystem() {}

void EventSystem::dispatchEvent(IEvent& event) {
	EventType type = event.type();
	for (auto listener : listeners[type]) {
		listener->handle(event);
	}
}