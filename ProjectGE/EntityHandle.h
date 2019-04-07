#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "Handle.h"
#include "EntityManager.h"

namespace Core {
	/* A handle with additional functionality appropriate for an Entity. */
	class EntityHandle : public Handle {
	public:
		EntityHandle(Entity entity, EntityManager* manager);
		EntityHandle(const Handle& handle);
		EntityHandle();
		~EntityHandle();

		void										setParent(const Entity& entity);

		template<typename T, class... Args> void	addComponent(Args&&... args);
		template<typename T> void					addComponent(T* component);
		template<typename T> void					removeComponent();
		void										destroy();
	};

	// --------------------------- Template Function Definitions --------------------------------
	template<typename T, class... Args>
	void EntityHandle::addComponent(Args&&... args) {
		return manager->addComponent(entity, new T(args...));
	}

	template<typename T>
	void EntityHandle::addComponent(T* component) {
		return manager->addComponent(entity, component);
	}

	template<typename T>
	void EntityHandle::removeComponent() {
		return manager->removeComponent<T>(entity);
	}
}
#endif