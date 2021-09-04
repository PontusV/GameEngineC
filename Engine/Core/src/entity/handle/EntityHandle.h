#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "Handle.h"
#include "entity/EntityManager.h"
#include <string>

namespace Core {
	/* An Entity handle with additional functionality. */
	class EntityHandle : public Handle {
	public:
		EntityHandle(Entity entity, EntityManager* entityManager);
		EntityHandle(Entity entity, EntityManager* entityManager, EntityLocationDetailed location);
		EntityHandle(const Handle& handle);
		EntityHandle();
		~EntityHandle();

		/* Adds the component at the end of the frame. Returns a temporary pointer to the component (it will become invalid after the end of the frame)*/
		template<typename T, class... Args>
		T* addComponent(Args&&... args);
		/* Adds the component at the end of the frame. Returns a temporary pointer to the component (it will become invalid after the end of the frame)*/
		template<typename T>
		T* addComponent(T component);
		/* Removes the component of type T at the end of the frame. */
		template<typename T>
		void removeComponent();

		template<typename Archetype>
		void serialize(Archetype& ar) const {
			ar(entity);
		}
		template<typename Archetype>
		void deserialize(Archetype& ar) {
			ar(entity);
		}

		bool operator==(const EntityHandle& other) const;
		bool operator!=(const EntityHandle& other) const;
	};

	// --------------------------- Template Function Definitions --------------------------------
	template<typename T, class... Args>
	T* EntityHandle::addComponent(Args&&... args) {
		if (refresh()) {
			T component(args...);
			T* temp = entityManager->addComponentQueued(entity, component);
			return temp;
		}
		return nullptr;
	}

	template<typename T>
	T* EntityHandle::addComponent(T component) {
		if (refresh()) {
			T* temp = entityManager->addComponentQueued(entity, component);
			return temp;
		}
		return nullptr;
	}

	template<typename T>
	void EntityHandle::removeComponent() {
		if (refresh()) {
			entityManager->removeComponentQueued<T>(entity);
		}
	}
}
#endif