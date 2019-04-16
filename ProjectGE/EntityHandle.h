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
		template<typename T> void					addComponent(T component);
		template<typename T> void					removeComponent();
		void										destroy();
	};

	// --------------------------- Template Function Definitions --------------------------------
	template<typename T, class... Args>
	void EntityHandle::addComponent(Args&&... args) {
		T component(args...);
		manager->addComponent(entity, component);
		update();
	}

	template<typename T>
	void EntityHandle::addComponent(T component) {
		manager->addComponent(entity, component);
		update();
	}

	template<typename T>
	void EntityHandle::removeComponent() {
		manager->removeComponent<T>(entity);
		update();
	}
}
#endif