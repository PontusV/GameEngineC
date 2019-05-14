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
	manager->destroyEntityQueued(entity);
}

void EntityHandle::setParent(const Entity& entity) {
	setParent(manager->getEntityHandle(entity));
}

void EntityHandle::setParent(EntityHandle entity) {
	update();
	ParentEntity* parentComponent = getComponent<ParentEntity>();
	if (parentComponent) {
		parentComponent->setParent(entity);
	}
	else {
		addComponent<ParentEntity>(entity);
	}
}

bool EntityHandle::isChild(Entity entity) {
	std::size_t childCount = getChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		EntityHandle child = getChild(i);
		if (child.getEntity() == entity || child.isChild(entity))
			return true;
	}
	return false;
}
std::string EntityHandle::getEntityName() {
	return manager->getEntityName(entity);
}