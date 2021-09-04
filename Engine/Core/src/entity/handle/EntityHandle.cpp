#include "EntityHandle.h"
#include "components/entity/ParentEntity.h"
#include "entity/EntityManager.h"

using namespace Core;


EntityHandle::EntityHandle(Entity entity, EntityManager* entityManager) : Handle(entity, entityManager) {
}

EntityHandle::EntityHandle(Entity entity, EntityManager* entityManager, EntityLocationDetailed location) : Handle(entity, entityManager, location) {
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