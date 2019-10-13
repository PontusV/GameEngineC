#include "EntityHandle.h"
#include "ParentEntity.h"
#include "EntityManager.h"

using namespace Core;


EntityHandle::EntityHandle(Entity entity, Scene* scene) : Handle(entity, scene) {
}

EntityHandle::EntityHandle(const Handle& handle) : Handle(handle) {
}

EntityHandle::EntityHandle() {
} // constructor for invalid handle


EntityHandle::~EntityHandle() {
}

void EntityHandle::setParent(const Entity& entity) {
	EntityHandle parent = scene->getEntityHandle(entity);
	setParent(parent);
}

void EntityHandle::setParent(EntityHandle entity) {
	scene->setParentQueued(*this, entity);
}

std::string EntityHandle::getEntityName() {
	if (refresh()) {
		return scene->getEntityManager()->getEntityName(entity);
	}
	std::cout << "EntityHandle::getEntityName::ERROR Invalid Handle!";
	return "Invalid";
}

HideFlags EntityHandle::getEntityHideFlags() {
	HideFlags result = scene->getEntityManager()->getEntityHideFlags(entity);
	EntityHandle parent = getParent();
	while (parent.refresh()) {
		HideFlags hideFlags = parent.getEntityHideFlags();
		result = result | hideFlags;
		parent = parent.getParent();
	}
	return result;
}

void EntityHandle::setEntityHideFlags(HideFlags hideFlags) {
	return scene->getEntityManager()->setEntityHideFlags(entity, hideFlags);
}

bool EntityHandle::operator==(const EntityHandle& other) const {
	return entity.getID() == other.entity.getID();
}
bool EntityHandle::operator!=(const EntityHandle& other) const {
	return !(*this == other);
}

void EntityHandle::removeComponent(ComponentTypeID componentTypeID) {
	scene->removeComponentQueued(entity, componentTypeID);
}