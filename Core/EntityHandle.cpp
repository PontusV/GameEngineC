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

bool EntityHandle::operator==(const EntityHandle& other) const {
	return entity.getID() == other.entity.getID();
}
bool EntityHandle::operator!=(const EntityHandle& other) const {
	return !(*this == other);
}

void EntityHandle::removeComponent(ComponentTypeID componentTypeID) {
	scene->removeComponentQueued(entity, componentTypeID);
}