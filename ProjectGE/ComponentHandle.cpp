#include "ComponentHandle.h"
#include <iostream>

using namespace Core;

ComponentHandle::ComponentHandle(ComponentTypeID typeID, Entity entity, EntityManager* manager) : typeID(typeID), Handle(entity, manager) {
	if (!isValid())
		std::cout << "ComponentHandle::Error The ChunkEntityHandle is invalid!\n";
}

ComponentHandle::ComponentHandle(ComponentTypeID typeID, Handle owner) : typeID(typeID), Handle(owner) {
	if (!isValid())
		std::cout << "ComponentHandle::Error The ChunkEntityHandle is invalid!\n";
}
ComponentHandle::ComponentHandle() {
}

ComponentHandle::~ComponentHandle() {
}

/* Will return nullptr if ChunkEntityHandle is invalid, which means the Entity has been moved to another Chunk or got destroyed. */
Component* ComponentHandle::getComponent() {
	std::vector<Component*> components = getComponents();
	return getFirstDerivingComponent(components, typeID);
}