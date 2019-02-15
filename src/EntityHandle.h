#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "Entity.h"

namespace GameEngine {
	class HandleManager;

	class EntityHandle {
	public:
		EntityHandle(Entity entity);
		~EntityHandle();

		template<typename T> T getComponent();
		template<typename T, class... Args> void addComponent(Args&&... args);
		template<typename T> void removeComponent();
	private:
		Entity entity;
	};

	/* Returns a copy of the component attached to this entity. */
	template<typename T>
	T EntityHandle::getComponent() {
		//ComponentManager<T>& manager = ComponentManager<T>::getInstance();
		//Handle cmpHandle = manager.get(entity);
		//return *handleManager.get<T>(cmpHandle);
		return T();
	}

	template<typename T, class... Args>
	void EntityHandle::addComponent(Args&&... args) {
		//ComponentManager<T>& manager = ComponentManager<T>::getInstance();
		//manager.add(entity, std::make_shared<T>(args...), handleManager);
	}

	template<typename T>
	void EntityHandle::removeComponent() {
		//ComponentManager<T>& manager = ComponentManager<T>::getInstance();
		//manager.remove(entity, handleManager);
	}
}
#endif