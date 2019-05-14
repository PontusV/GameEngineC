#ifndef COMPONENT_HANDLE_H
#define COMPONENT_HANDLE_H

#include "Handle.h"

namespace Core {
	/* A handle for a component with a specific ComponentTypeID. Does not look for components with a type deriving from the type with the specified ComponentTypeID. */
	class ComponentHandle : public Handle {
	public:
		ComponentHandle(ComponentTypeID typeID, Entity entity, EntityManager* manager);
		ComponentHandle(ComponentTypeID typeID, Handle owner);
		ComponentHandle();
		~ComponentHandle();

		Component* getComponent();

	private:
		ComponentTypeID typeID;
	};
}
#endif