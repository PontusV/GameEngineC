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
	manager->destroyEntity(entity);
}

#include "Transform.h"
void EntityHandle::setParent(const Entity& entity) {
	update();
	ParentEntity* parentComponent = getComponent<ParentEntity>();
	if (parentComponent) {
		parentComponent->setParent(manager->getEntityHandle(entity));
	}
	else {
		addComponent<ParentEntity>(manager->getEntityHandle(entity));
	}
}