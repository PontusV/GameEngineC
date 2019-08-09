#ifndef COMPONENT_HANDLE_H
#define COMPONENT_HANDLE_H

#include "Handle.h"
#include "Component.h"

namespace Core {
	/* A handle for a component with a specific ComponentTypeID. Does not look for components with a type deriving from the type with the specified ComponentTypeID. */
	class ComponentHandle {
	public:
		ComponentHandle(ComponentID id, Entity entity, EntityManager* manager);
		ComponentHandle(ComponentID id, Handle owner);
		ComponentHandle(Component* instance);
		ComponentHandle();
		~ComponentHandle();

		Component* getComponent();
		bool isValid();

	private:
		Handle owner;
		ComponentID id = 0;
	};
}
#endif