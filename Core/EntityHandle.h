#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "Handle.h"
#include "EntityManager.h"
#include <string>

namespace Core {
	/* A handle with additional functionality appropriate for an Entity. */
	class EntityHandle : public Handle {
	public:
		EntityHandle(Entity entity, EntityManager* manager);
		EntityHandle(const Handle& handle);
		EntityHandle();
		~EntityHandle();

		void setParent(const Entity& entity);
		void setParent(EntityHandle entity);
		/* Returns true if the given Entity is a child of the Entity this instance points to. */
		bool isChild(Entity entity);

		/* Adds the component at the end of the frame. Returns a temporary pointer to the component (it will become invalid after the end of the frame)*/
		template<typename T, class... Args>
		T* addComponent(Args&&... args);
		/* Adds the component at the end of the frame. Returns a temporary pointer to the component (it will become invalid after the end of the frame)*/
		template<typename T>
		T* addComponent(T component);
		/* Removes the component of type T at the end of the frame. */
		template<typename T>
		void removeComponent();
		/* Removes the Entity at the end of the frame. */
		void destroy();

		std::string getEntityName();

		template<typename... Ts>
		EntityHandle createEntity(std::string name, Ts&... components);
	};

	// --------------------------- Template Function Definitions --------------------------------
	template<typename T, class... Args>
	T* EntityHandle::addComponent(Args&&... args) {
		T component(args...);
		T* temp = manager->addComponentQueued(entity, component);
		return temp;
	}

	template<typename T>
	T* EntityHandle::addComponent(T component) {
		T* temp = manager->addComponentQueued(entity, component);
		return temp;
	}

	template<typename T>
	void EntityHandle::removeComponent() {
		manager->removeComponentQueued<T>(entity);
	}

	template<typename... Ts>
	EntityHandle EntityHandle::createEntity(std::string name, Ts&... components) {
		Entity entity = manager->createEntityQueued(name, components...);
		return EntityHandle(entity, manager);
	}
}
#endif