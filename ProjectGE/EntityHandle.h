#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "Entity.h"
#include "EntityManager.h"
#include <memory>
#include <stdexcept>

namespace GameEngine {
	class HandleManager;

	class EntityHandle {
	public:
		EntityHandle(Entity entity, std::weak_ptr<EntityManager> manager);
		EntityHandle();
		~EntityHandle();

		template<typename T> T* getComponent();
		template<typename T, class... Args> void addComponent(Args&&... args);
		template<typename T> void removeComponent();
		EntityHandle& operator=(const EntityHandle& other) {
			entity = other.entity;
			manager = other.manager;
			return *this;
		}
	private:
		Entity entity;
		std::weak_ptr<EntityManager> manager;
	};

	/* Returns a copy of the component attached to this entity. */
	template<typename T>
	T* EntityHandle::getComponent() {
		if (auto spt = manager.lock())
			return static_cast<T*>(spt->getComponent<T>(entity));
		throw std::invalid_argument("The EntityHandle is no longer valid! EntityHandle does not a valid pointer to an EntityManager.");
	}

	template<typename T, class... Args>
	void EntityHandle::addComponent(Args&&... args) {
		if (auto spt = manager.lock())
			return spt->addComponent(entity, new T(args...));
		throw std::invalid_argument("The EntityHandle is no longer valid! EntityHandle does not a valid pointer to an EntityManager.");
	}

	template<typename T>
	void EntityHandle::removeComponent() {
		if (auto spt = manager.lock())
			return spt->removeComponent<T>(entity);
		throw std::invalid_argument("The EntityHandle is no longer valid! EntityHandle does not a valid pointer to an EntityManager.");
	}
}
#endif