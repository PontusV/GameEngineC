#include "ComponentHandle.h"
#include "Scene.h"
#include <iostream>

using namespace Core;

ComponentHandle::ComponentHandle(ComponentTypeID id, Entity entity, Scene* scene) : id(id), owner(Handle(entity, scene)) {
}

ComponentHandle::ComponentHandle(ComponentTypeID id, Handle owner) : id(id), owner(Handle(owner)) {
}

ComponentHandle::ComponentHandle(Component* instance) : id(instance->getComponentID()), owner(instance->getOwner()) {
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