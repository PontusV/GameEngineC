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
unsigned char EntityHandle::getEntityLayer() const {
	return 0; // TEMP
}


void EntityHandle::setEntityHideFlags(HideFlags hideFlags) {
	return manager->setEntityHideFlags(entity, hideFlags);
}