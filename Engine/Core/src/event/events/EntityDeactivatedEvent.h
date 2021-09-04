#ifndef ENTITY_DEACTIVATED_EVENT_H
#define ENTITY_DEACTIVATED_EVENT_H

#include "event/IEvent.h"
#include "entity/handle/Handle.h"

namespace Core {

	struct EntityDeactivatedEvent : public IEvent {
		static constexpr EventType descriptor = "EntityDeactivatedEvent";
		EventType type() const override { return descriptor; }
		Handle entityHandle;
	};
}
#endif