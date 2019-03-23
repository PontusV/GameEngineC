#ifndef EVENT_LISTENER
#define EVENT_LISTENER

#include <string>

namespace Core {
	class EventListener
	{
	public:
		EventListener();
		~EventListener();

		virtual void callEvent(std::string eventName) = 0;
	};
}
#endif