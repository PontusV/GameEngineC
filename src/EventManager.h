#ifndef EVENT_MANAGER
#define EVENT_MANAGER
#include <string>
#include <vector>

class EventListener;

class EventManager
{
public:
	EventManager();
	~EventManager();

	void registerEventListener(EventListener* listener, std::string eventName);
	void callEvent(std::string eventName);
private:
	std::vector<EventListener*> listeners;
};
#endif