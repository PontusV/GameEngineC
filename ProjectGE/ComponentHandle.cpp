#include "ComponentHandle.h"
#include "HandleManager.h"

using namespace Core;

ComponentHandle::ComponentHandle(ChunkEntityHandle owner, ComponentTypeID typeID) : owner(owner), typeID(typeID) {
	if (!owner.isValid())
		std::cout << "ComponentHandle::Error The ChunkEntityHandle is invalid!\n";
}

ComponentHandle::~ComponentHandle() {
}

/* Will return nullptr if ChunkEntityHandle is invalid, which means the Entity has been moved to another Chunk or got destroyed. */
Component* ComponentHandle::getComponent() {
	// Check if entity has moved
	//if (entityPtr && *entityPtr == owner.getEntity()) { // This code is faulty
	//	return componentPtr;
	//}
	// The entity has moved, reinit ptrs
	entityPtr = owner.getEntityPtr();
	componentPtr = owner.getComponent(typeID);
	return componentPtr;
}

bool ComponentHandle::isValid() {
	// If owner is valid the ptrs can be initialized & validated
	return owner.isValid();
}

Entity ComponentHandle::getOwner() {
	return owner.getEntity();
}