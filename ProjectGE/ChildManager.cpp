#include "ChildManager.h"
#include "Behaviour.h"

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
	// Notify scripts
	std::vector<Behaviour*> scripts = owner.getComponents<Behaviour>();
	for (Behaviour* script : scripts) {
		script->onChildrenChanged();
	}
}

void ChildManager::childRemoved(Entity entity) {
	for (std::size_t i = 0; i < children.size(); i++) {
		if (children[i].getEntity() == entity) {
			children.erase(children.begin() + i);
		}
	}
	// Notify scripts
	std::vector<Behaviour*> scripts = owner.getComponents<Behaviour>();
	for (Behaviour* script : scripts) {
		script->onChildrenChanged();
	}
}

std::size_t ChildManager::getChildCount() {
	return children.size();
}

Handle ChildManager::getChild(std::size_t index) {
	if (index >= children.size())
		return Handle();
	Handle& child = children[index];
	child.refresh(); // Make sure it is updated
	return child;
}

std::vector<Handle>& ChildManager::getChildren() {
	return children;
}