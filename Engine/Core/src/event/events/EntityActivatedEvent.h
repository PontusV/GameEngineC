#ifndef ENTITY_ACTIVATED_EVENT_H
#define ENTITY_ACTIVATED_EVENT_H

#include "event/IEvent.h"
#include "entity/handle/Handle.h"

namespace Core {

	struct EntityActivatedEvent : public IEvent {
		static constexpr EventType descriptor = "EntityActivatedEvent";
		EventType type() const override { return descriptor; }
		Handle entityHandle;
	};
}
#endif