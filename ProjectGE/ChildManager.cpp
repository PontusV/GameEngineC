#include "ChildManager.h"

using namespace Core;


ChildManager::~ChildManager()
{
}

void ChildManager::childAdded(Handle entity) {
	// Check if update
	for (Handle& child : children) {
		if (child.getEntity() == entity.getEntity()) {
			child = entity; // Update child reference
		}
	}
	// No update
	children.push_back(entity);
}

void ChildManager::childRemoved(Entity entity) {
	for (std::size_t i = 0; i < children.size(); i++) {
		if (children[i].getEntity() == entity) {
			children.erase(children.begin() + i);
		}
	}
}

std::size_t ChildManager::getChildCount() {
	return children.size();
}

Handle* ChildManager::getChild(std::size_t index) {
	if (index >= children.size())
		return nullptr;
	return &children[index];
}

std::vector<Handle>& ChildManager::getChildren() {
	return children;
}