#include "Handle.h"
#include "EntityManager.h"
#include "Chunk.h"
#include "Component.h"

#include "ParentEntity.h"
#include "ChildManager.h"

using namespace Core;

Handle::Handle(Entity entity, EntityManager* manager) : entity(entity), manager(manager) {
}

Handle::Handle() {
} // Creates invalid handle


Handle::~Handle() {
}

const Entity& Handle::getEntity() const {
	return entity;
}

bool Handle::isValid() {
	// If not valid call update and check for validity again
	if (auto chunk = locationData.chunk.lock())
		return chunk->getEntityArrayPtr()[locationData.index] == entity;
	return false;
}

void Handle::update() {
	if (manager)
		locationData = manager->getLocation(entity);
}

void Handle::updateLocation(EntityLocation location) {
	locationData = location;
}

const EntityLocation& Handle::getLocation() const {
	return locationData;
}

std::vector<Component*> Handle::getComponents() {
	if (isValid()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index);
	}
	return std::vector<Component*>(); // Empty
}

std::vector<Component*> Handle::getComponentsInChildren() {
	std::vector<Component*> components;
	if (isValid()) {
		std::size_t childCount = getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle* child = getChild(i);
			std::vector<Component*> childComponents = child->getComponents();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
	}
	return components;
}

std::vector<Component*> Handle::getComponentsInParent() {
	std::vector<Component*> components;
	if (isValid()) {
		Handle* parent = getParent();
		if (parent) {
			std::vector<Component*> parentComponents = parent->getComponents();
			components.insert(components.end(), parentComponents.begin(), parentComponents.end());
			parentComponents = parent->getComponentsInParent();
			components.insert(components.end(), parentComponents.begin(), parentComponents.end());
		}
	}
	return components;
}

Handle* Handle::getParent() {
	ParentEntity* parentCmp = getComponent<ParentEntity>();
	if (parentCmp) {
		return parentCmp->getParent();
	}
	return nullptr;
}

Handle* Handle::getChild(std::size_t index) {
	ChildManager* childManager = getComponent<ChildManager>();
	if (childManager) {
		return childManager->getChild(index);
	}
	return nullptr;
}

std::size_t Handle::getChildCount() {
	ChildManager* childManager = getComponent<ChildManager>();
	if (childManager) {
		return childManager->getChildCount();
	}
	return 0;
}

/* Makes use of Component::equalType to get component deriving from T as well. */
Component* Handle::getFirstDerivingComponent(std::vector<Component*> components, ComponentTypeID typeID) {
	for (Component* component : components) {
		if (component->equalType(typeID)) {
			return component;
		}
	}
	return nullptr;
}