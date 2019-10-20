#ifndef COMPONENT_HANDLE_H
#define COMPONENT_HANDLE_H

#include "Handle.h"
#include "entity/component/Component.h"

namespace Core {
	class Scene;

	/* A handle for a component with a specific ComponentTypeID. Does not look for components with a type deriving from the type with the specified ComponentTypeID. */
	template<typename T>
	class TComponentHandle {
	public:
		TComponentHandle(ComponentTypeID id, Entity entity, Scene* scene) : id(id), owner(Handle(entity, scene)) {}
		TComponentHandle(ComponentTypeID id, Handle owner) : id(id), owner(Handle(owner)) {}
		TComponentHandle(T* instance) : id(instance->getComponentID()), owner(instance->getOwner()) {
			static_assert(std::is_base_of<Component, T>::value || std::is_same<Component, T>::value, "Not an instance of Component");
		}
		TComponentHandle() {}
		virtual ~TComponentHandle() {}

		T* get() {
			for (T* component : owner.getComponents<T>()) {
				if (component->getComponentID() == id) return component;
			}
			return nullptr;
		}
		bool isValid() {
			return owner.refresh();
		}
		void clear() {
			owner = Handle();
			id = 0;
		}

	private:
		Handle owner;
		ComponentID id = 0;
	};

	/* A handle for a component with a specific ComponentTypeID. Does not look for components with a type deriving from the type with the specified ComponentTypeID. */
	class ComponentHandle : public TComponentHandle<Component> {
	public:
		ComponentHandle(ComponentID id, Entity entity, Scene* scene) : TComponentHandle<Component>(id, entity, scene) {}
		ComponentHandle(ComponentID id, Handle owner) : TComponentHandle<Component>(id, owner) {}
		ComponentHandle(Component* instance) : TComponentHandle<Component>(instance) {}
		ComponentHandle() {}
		~ComponentHandle() {}

		Component* getComponent() {
			return TComponentHandle<Component>::get();
		}

		template<typename T>
		T* getComponent() {
			return static_cast<T*>(TComponentHandle<Component>::get());
		}
	};
}
#endif