#ifndef COMPONENT_HANDLE_H
#define COMPONENT_HANDLE_H

#include "Handle.h"

namespace Core {
	class ComponentHandle : public Handle {
	public:
		ComponentHandle(ComponentTypeID typeID, Entity entity, EntityManager* manager);
		ComponentHandle(ComponentTypeID typeID, Handle owner);
		ComponentHandle();
		~ComponentHandle();

		Component*		getComponent();

	private:
		ComponentTypeID typeID;
	};
}
#endif