#ifndef INTERFACE_EVENT_LISTENER_H
#define INTERFACE_EVENT_LISTENER_H

namespace Core {
	struct IEvent;

	struct IEventListener {
		virtual ~IEventListener();
		virtual void handle(IEvent& event) = 0;
	};
}
#endif