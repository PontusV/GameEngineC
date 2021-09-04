#ifndef ENTITY_CREATED_EVENT_H
#define ENTITY_CREATED_EVENT_H

#include "event/IEvent.h"
#include "entity/handle/Handle.h"

namespace Core {

	struct EntityCreatedEvent : public IEvent {
		static constexpr EventType descriptor = "EntityCreatedEvent";
		EventType type() const override { return descriptor; }
		Handle entityHandle;
	};
}
#endif