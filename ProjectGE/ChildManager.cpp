#include "ChildManager.h"

using namespace Core;


ChildManager::ChildManager(std::istream& is) {
}
ChildManager::~ChildManager()
{
}

void ChildManager::childAdded(ChunkEntityHandle entity) {
	children.push_back(entity);
	for (ChildListener* listener : listeners) {
		listener->childAdded(entity);
	}
}

void ChildManager::childRemoved(Entity entity) {
	auto it = std::find(children.begin(), children.end(), entity);
	if (it != children.end())
		children.erase(it);

	for (ChildListener* listener : listeners) {
		listener->childRemoved(entity);
	}
}

void ChildManager::registerListener(ChildListener* listener) {
	// Notify of existing children
	for (ChunkEntityHandle& child : children) {
		listener->childAdded(child);
	}

	listeners.push_back(listener);
}

void ChildManager::unregisterListener(ChildListener* listener) {
	if (listeners.size() == 0) return;
	for (auto it = listeners.begin(); it != listeners.end(); it++) {
		if (listener == *it) {
			listeners.erase(it);
			break;
		}
	}
}

std::size_t ChildManager::getChildCount() {
	return children.size();
}

ChunkEntityHandle* ChildManager::getChild(std::size_t index) {
	if (index >= children.size())
		return nullptr;
	return &children[index];
}

std::vector<ChunkEntityHandle>& ChildManager::getChildren() {
	return children;
}

// Save & Load operator
void ChildManager::serialize(std::ostream& os) const {

}

void ChildManager::deserialize(std::istream& is) {

}