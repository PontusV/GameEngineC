#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "Entity.h"

class HandleManager;

class EntityHandle
{
public:
	EntityHandle(Entity entity, HandleManager& handleManager);
	~EntityHandle();

	template<typename T> T getComponent();
	template<typename T, class... Args> void addComponent(Args&&... args);
	template<typename T> void removeComponent();
	template<typename T> void updateComponent(T& component);
private:
	Entity entity;
	HandleManager& handleManager;
};

#include "ComponentManager.h"
#include "Handle.h"
#include "HandleManager.h"
#include <memory>

/* Returns a copy of the component attached to this entity. */
template<typename T>
T EntityHandle::getComponent() {
	ComponentManager<T>& manager = ComponentManager<T>::getInstance();
	Handle cmpHandle = manager.get(entity);
	return *handleManager.get<T>(cmpHandle);
}

template<typename T, class... Args>
void EntityHandle::addComponent(Args&&... args) {
	ComponentManager<T>& manager = ComponentManager<T>::getInstance();
	manager.add(entity, std::make_shared<T>(args...), handleManager);
}

template<typename T>
void EntityHandle::removeComponent() {
	ComponentManager<T>& manager = ComponentManager<T>::getInstance();
	manager.remove(entity, handleManager);
}

/* Updates component with the data from the given component. */
template<typename T>
void EntityHandle::updateComponent(T& component) {
	ComponentManager<Transform>::getInstance().update(entity, component, handleManager);
}
#endif