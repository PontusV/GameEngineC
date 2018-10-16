#ifndef EVENT_LISTENER
#define EVENT_LISTENER

#include <string>

class EventListener
{
public:
	EventListener();
	~EventListener();

	virtual void callEvent(std::string eventName) = 0;
};
#endif