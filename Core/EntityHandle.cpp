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

void EntityHandle::setParent(const Entity& entity) {
	EntityHandle parent = manager->getEntityHandle(entity);
	setParent(parent);
}

void EntityHandle::setParent(EntityHandle entity) {
	manager->setParentQueued(*this, entity);
}

std::string EntityHandle::getEntityName() {
	if (refresh()) {
		return manager->getEntityName(entity);
	}
	std::cout << "EntityHandle::getEntityName::ERROR Invalid Handle!";
	return "Invalid";
}

HideFlags EntityHandle::getEntityHideFlags() {
	HideFlags result = manager->getEntityHideFlags(entity);
	EntityHandle parent = getParent();
	while (parent.refresh()) {
		HideFlags hideFlags = parent.getEntityHideFlags();
		result = result | hideFlags;
		parent = parent.getParent();
	}
	return result;
}

void EntityHandle::setEntityHideFlags(HideFlags hideFlags) {
	return manager->setEntityHideFlags(entity, hideFlags);
}

bool EntityHandle::operator==(const EntityHandle& other) {
	return entity.getID() == other.entity.getID();
}
bool EntityHandle::operator!=(const EntityHandle& other) {
	return !(*this == other);
}