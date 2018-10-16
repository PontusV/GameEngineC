#include "EntityHandle.h"



EntityHandle::EntityHandle(Entity entity, HandleManager& handleManager) : entity(entity), handleManager(handleManager)
{
}


EntityHandle::~EntityHandle()
{
}
