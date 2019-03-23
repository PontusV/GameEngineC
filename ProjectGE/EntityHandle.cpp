#include "EntityHandle.h"
using namespace Core;


EntityHandle::EntityHandle(Entity entity, std::weak_ptr<EntityManager> manager) : entity(entity), manager(manager)
{
}

EntityHandle::EntityHandle() : manager(std::weak_ptr<EntityManager>()) {}


EntityHandle::~EntityHandle()
{
}

void EntityHandle::destroy() {
	if (auto spt = manager.lock())
		return spt->removeEntity(entity);
	throw std::invalid_argument("The EntityHandle is no longer valid! EntityHandle does not have a valid pointer to an EntityManager.");
}