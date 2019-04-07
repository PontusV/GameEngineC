#include "EntityHandle.h"
#include "ParentEntity.h"
#include "EntityManager.h"
using namespace Core;


EntityHandle::EntityHandle(Entity entity, EntityManager* manager) : Handle(entity, manager) {
}

EntityHandle::EntityHandle(const Handle& handle) : Handle(handle) {
}

EntityHandle::EntityHandle() {
} // constructor for invalid handle


EntityHandle::~EntityHandle() {
}

void EntityHandle::destroy() {
	return manager->destroyEntity(entity);
}

void EntityHandle::setParent(const Entity& entity) {
	addComponent<ParentEntity>(manager->getEntityHandle(entity));
}