#ifndef INTERFACE_EVENT_H
#define INTERFACE_EVENT_H

namespace Core {
	typedef const char* EventType;
	struct IEvent {
		virtual ~IEvent() {};
		virtual EventType type() const = 0;
	};
}
#endif