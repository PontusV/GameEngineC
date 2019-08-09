#include "ComponentHandle.h"
#include <iostream>

using namespace Core;

ComponentHandle::ComponentHandle(ComponentTypeID id, Entity entity, EntityManager* manager) : id(id), owner(Handle(entity, manager)) {
	if (!owner.refresh())
		std::cout << "ComponentHandle::Constructor1::Error The EntityHandle(" << owner.getEntity().getID() << ") is invalid!\n";
}

ComponentHandle::ComponentHandle(ComponentTypeID id, Handle owner) : id(id), owner(Handle(owner)) {
	if (!owner.refresh())
		std::cout << "ComponentHandle::Constructor2::Error The EntityHandle(" << owner.getEntity().getID() << ") is invalid!\n";
}

ComponentHandle::ComponentHandle(Component* instance) : id(instance->getComponentID()), owner(instance->getOwner()) {
	if (!owner.refresh())
		std::cout << "ComponentHandle::Constructor3::Error The EntityHandle(" << owner.getEntity().getID() << ") is invalid!\n";
}

ComponentHandle::ComponentHandle() {
}

ComponentHandle::~ComponentHandle() {
}

/* Will return nullptr if ChunkEntityHandle is invalid, which means the Entity has been moved to another Chunk or got destroyed. */
Component* ComponentHandle::getComponent() {
	for (Component* component : owner.getComponents()) {
		if (component->getComponentID() == id) return component;
	}
	return nullptr;
}

bool ComponentHandle::isValid() {
	return owner.refresh();
}