#ifndef COMPONENT_HANDLE_H
#define COMPONENT_HANDLE_H

#include "Handle.h"
#include "Component.h"

namespace Core {
	class Scene;
	/* A handle for a component with a specific ComponentTypeID. Does not look for components with a type deriving from the type with the specified ComponentTypeID. */
	class ComponentHandle {
	public:
		ComponentHandle(ComponentID id, Entity entity, Scene* scene);
		ComponentHandle(ComponentID id, Handle owner);
		ComponentHandle(Component* instance);
		ComponentHandle();
		~ComponentHandle();

		Component* getComponent();
		template<typename T>
		T* getComponent();
		bool isValid();

	private:
		Handle owner;
		ComponentID id = 0;
	};

	template<typename T>
	T* ComponentHandle::getComponent() {
		return static_cast<T*>(getComponent());
	}
}
#endif