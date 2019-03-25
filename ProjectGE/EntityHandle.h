#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "Entity.h"
#include "EntityManager.h"
#include <memory>
#include <stdexcept>

namespace Core {
	class HandleManager;

	class EntityHandle {
	public:
		EntityHandle(Entity entity, std::weak_ptr<EntityManager> manager);
		EntityHandle();
		~EntityHandle();

		template<typename T> T*						getComponent();
		template<typename T, class... Args> void	addComponent(Args&&... args);
		template<typename T> void					addComponent(T* component);
		template<typename T> void					removeComponent();
		void										destroy();

		const Entity&	getEntity() { return entity; }
		bool			isValid()	{ return (manager.lock() != nullptr && entity.getID() != Entity::INVALID_ID); }

		EntityHandle& operator=(const EntityHandle& other) {
			entity = other.entity;
			manager = other.manager;
			return *this;
		}
	private:
		Entity entity;
		std::weak_ptr<EntityManager> manager;
	};

	// --------------------------- Template Function Definitions --------------------------------

	template<typename T>
	T* EntityHandle::getComponent() {
		if (auto spt = manager.lock())
			return spt->getComponent<T>(entity);
		throw std::invalid_argument("The EntityHandle is no longer valid! EntityHandle does not have a valid pointer to an EntityManager.");
	}

	template<typename T, class... Args>
	void EntityHandle::addComponent(Args&&... args) {
		if (auto spt = manager.lock())
			return spt->addComponent(entity, new T(args...));
		throw std::invalid_argument("The EntityHandle is no longer valid! EntityHandle does not have a valid pointer to an EntityManager.");
	}

	template<typename T>
	void EntityHandle::addComponent(T* component) {
		if (auto spt = manager.lock())
			return spt->addComponent(entity, component);
		throw std::invalid_argument("The EntityHandle is no longer valid! EntityHandle does not have a valid pointer to an EntityManager.");
	}

	template<typename T>
	void EntityHandle::removeComponent() {
		if (auto spt = manager.lock())
			return spt->removeComponent<T>(entity);
		throw std::invalid_argument("The EntityHandle is no longer valid! EntityHandle does not have a valid pointer to an EntityManager.");
	}
}
#endif