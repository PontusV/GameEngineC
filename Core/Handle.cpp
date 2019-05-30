#include "Handle.h"
#include "EntityManager.h"
#include "Chunk.h"
#include "Component.h"

#include "ParentEntity.h"
#include "ChildManager.h"

using namespace Core;

Handle::Handle(Entity entity, EntityManager* manager) : entity(entity), manager(manager) {
}

Handle::Handle() : entity(0) {
} // Creates invalid handle


Handle::~Handle() {
}

const Entity& Handle::getEntity() const {
	return entity;
}

bool Handle::isValid() {
	if (auto chunk = locationData.chunk.lock()) {
		return chunk->getEntity(locationData.index) == entity;
	}
	return false;
}

bool Handle::refresh() {
	if (!isValid()) {
		update();
		return isValid();
	}
	return true;
}

void Handle::update() {
	if (manager) {
		locationData = manager->getLocation(entity);
	}
}

void Handle::updateLocation(EntityLocation location) {
	locationData = location;
}

const EntityLocation& Handle::getLocation() const {
	return locationData;
}

Component* Handle::getComponent(ComponentTypeID typeID) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponent(locationData.index, typeID);
	}
	return nullptr;
}

Component* Handle::getComponent(ComponentType type) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponent(locationData.index, type);
	}
	return nullptr;
}

std::vector<Component*> Handle::getComponents(ComponentType type) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index, type);
	}
	return std::vector<Component*>(); // Empty
}

std::vector<Component*> Handle::getComponents() {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index);
	}
	return std::vector<Component*>(); // Empty
}

std::vector<Component*> Handle::getComponentsInChildren() {
	std::vector<Component*> components;
	std::size_t childCount = getChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = getChild(i);
		std::vector<Component*> childComponents = child.getComponents();
		components.insert(components.end(), childComponents.begin(), childComponents.end());
	}
	return components;
}

std::vector<Component*> Handle::getComponentsInParents() {
	std::vector<Component*> components;
	Handle parent = getParent();
	if (parent.refresh()) {
		std::vector<Component*> parentComponents = parent.getComponents();
		components.insert(components.end(), parentComponents.begin(), parentComponents.end());
		parentComponents = parent.getComponentsInParents();
		components.insert(components.end(), parentComponents.begin(), parentComponents.end());
	}
	return components;
}

std::vector<Component*> Handle::getComponentsUpwards() {
	std::vector<Component*> components = getComponents();
	std::vector<Component*> parentComponents = getComponentsInParents();
	components.insert(components.end(), parentComponents.begin(), parentComponents.end());
	return components;
}

std::vector<Component*> Handle::getComponentsDownwards() {
	std::vector<Component*> components = getComponents();
	std::vector<Component*> childComponents = getComponentsInChildren();
	components.insert(components.end(), childComponents.begin(), childComponents.end());
	return components;
}

Handle Handle::getParent() {
	ParentEntity* parentCmp = static_cast<ParentEntity*>(getComponent(typeIDof(ParentEntity))); // Get component with ComponentTypeID is much faster
	if (parentCmp) {
		return parentCmp->getParent();
	}
	return Handle();
}

Handle Handle::getChild(std::size_t index) {
	ChildManager* childManager = static_cast<ChildManager*>(getComponent(typeIDof(ChildManager))); // Get component with ComponentTypeID is much faster
	if (childManager) {
		std::size_t immediateChildCount = childManager->getChildCount();
		// Check if child is immediate
		if (index < immediateChildCount) {
			return childManager->getChild(index);
		} // Otherwise check for the child in immediate children
		else {
			index -= immediateChildCount;
			for (std::size_t i = 0; i < immediateChildCount; i++) {
				Handle child = getChild(i);
				std::size_t childChildCount = child.getChildCount();
				if (index < childChildCount) {
					return child.getChild(index);
				}
				else {
					index -= childChildCount;
				}
			}
		}
	}
	return Handle();
}
std::size_t Handle::getImmediateChildCount() {
	ChildManager* childManager = static_cast<ChildManager*>(getComponent(typeIDof(ChildManager))); // Get component with ComponentTypeID is much faster
	if (childManager) {
		return childManager->getChildCount();
	}
	return 0;
}

std::size_t Handle::getChildCount() {
	std::size_t immediateChildCount = getImmediateChildCount();
	std::size_t childCount = immediateChildCount;
	for (std::size_t i = 0; i < immediateChildCount; i++) {
		childCount += getChild(i).getChildCount();
	}
	return childCount;
}