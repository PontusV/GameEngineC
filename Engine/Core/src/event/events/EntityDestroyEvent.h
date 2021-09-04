#ifndef ENTITY_DESTROY_EVENT_H
#define ENTITY_DESTROY_EVENT_H

#include "event/IEvent.h"
#include "entity/handle/Handle.h"

namespace Core {

	struct EntityDestroyEvent : public IEvent {
		static constexpr EventType descriptor = "EntityDestroyEvent";
		EventType type() const override { return descriptor; }
		Handle entityHandle;
	};
}
#endif