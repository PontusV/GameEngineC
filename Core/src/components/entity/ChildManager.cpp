#include "ChildManager.h"

using namespace Core;


ChildManager::~ChildManager()
{
}

void ChildManager::onChildAdded(Handle entity) {
	children.push_back(entity);
}

void ChildManager::onChildRemoved(Handle entity) {
	for (auto it = children.begin(); it != children.end(); it++) {
		if (*it == entity) {
			children.erase(it);
			return;
		}
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