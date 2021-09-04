#ifndef ENTITY_CHANGED_ARCHETYPE_EVENT_H
#define ENTITY_CHANGED_ARCHETYPE_EVENT_H

#include "event/IEvent.h"
#include "entity/handle/Handle.h"

namespace Core {

	struct EntityChangedArchetypeEvent : public IEvent {
		static constexpr EventType descriptor = "EntityChangedArchetypeEvent";
		EventType type() const override { return descriptor; }
		Handle entityHandle;
	};
}
#endif