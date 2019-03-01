#include "EntityHandle.h"
using namespace GameEngine;


EntityHandle::EntityHandle(Entity entity, std::weak_ptr<EntityManager> manager) : entity(entity), manager(manager)
{
}

EntityHandle::EntityHandle() : manager(std::weak_ptr<EntityManager>()) {}


EntityHandle::~EntityHandle()
{
}
