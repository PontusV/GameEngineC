#ifndef ENTITY_DESTROYED_EVENT_H
#define ENTITY_DESTROYED_EVENT_H

#include "event/IEvent.h"
#include "entity/Entity.h"

namespace Core {

	struct EntityDestroyedEvent : public IEvent {
		static constexpr EventType descriptor = "EntityDestroyedEvent";
		EventType type() const override { return descriptor; }
		Entity entity;
	};
}
#endif