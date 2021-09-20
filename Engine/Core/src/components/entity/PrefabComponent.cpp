#include "PrefabComponent.h"
#include <algorithm>
using namespace Core;

PrefabComponent::PrefabComponent() {}
PrefabComponent::PrefabComponent(std::string filePath) : filePath(filePath) {}
PrefabComponent::~PrefabComponent() {}

void PrefabComponent::setFilePath(std::string filePath) {
	this->filePath = filePath;
}

std::string PrefabComponent::getFilePath() {
	return filePath;
}

bool PrefabComponent::isDirty() const {
	return dirty;
}

void PrefabComponent::setDirty(bool value) {
	dirty = value;
}

bool PrefabComponent::addPropertyOverride(PrefabPropertyOverride prefabPropertyOverride) {
	if (IComponentData* component = prefabPropertyOverride.targetComponent.getComponent()) {
		if (component->getType().getProperty(prefabPropertyOverride.targetPropertyName).name.empty()) {
			std::cout << "PrefabComponent::addPropertyOverride::ERROR The property (" << prefabPropertyOverride.targetPropertyName << ") does not exist in the type (" << prefabPropertyOverride.targetComponent.getComponentTypeID() << ") of the given component" << std::endl;
			return false;
		}
	}
	// Only allows a property to be overriden once
	auto is_equal = [&prefabPropertyOverride](PrefabPropertyOverride& override) { return override.targetComponent == prefabPropertyOverride.targetComponent && override.targetPropertyName == prefabPropertyOverride.targetPropertyName; };
	if (std::find_if(prefabPropertyOverrides.begin(), prefabPropertyOverrides.end(), is_equal) != prefabPropertyOverrides.end()) {
		//std::cout << "PrefabComponent::addPropertyOverride::ERROR A matching override already exists" << std::endl;
		return false;
	}
	prefabPropertyOverrides.push_back(prefabPropertyOverride);
	return true;
}

bool PrefabComponent::removePropertyOverride(PrefabPropertyOverride prefabPropertyOverride) {
	auto is_equal = [&prefabPropertyOverride](PrefabPropertyOverride& override) { return override.targetComponent == prefabPropertyOverride.targetComponent && override.targetPropertyName == prefabPropertyOverride.targetPropertyName; };
	auto it = std::find_if(prefabPropertyOverrides.begin(), prefabPropertyOverrides.end(), is_equal);
	if (it != prefabPropertyOverrides.end()) {
		prefabPropertyOverrides.erase(it);
		return true;
	}
	return false;
}

void PrefabComponent::clearPropertyOverrides() {
	prefabPropertyOverrides.clear();
}

void PrefabComponent::clearPropertyOverrides(Entity entity, std::size_t typeID) {
	for (auto it = prefabPropertyOverrides.begin(); it != prefabPropertyOverrides.end();) {
		if (it->targetComponent.getComponentTypeID() == typeID && it->targetComponent.getOwner().getEntity() == entity) {
			it = prefabPropertyOverrides.erase(it);
		}
		else {
			it++;
		}
	}
}

std::vector<PrefabPropertyOverride> PrefabComponent::getPropertyOverrides() const {
	return prefabPropertyOverrides;
}

void PrefabComponent::setConnectedEntities(std::vector<ConnectedEntity> connectedEntities) {
	this->connectedEntities = connectedEntities;
}

std::vector<ConnectedEntity> PrefabComponent::getConnectedEntities() const {
	return connectedEntities;
}

void PrefabComponent::removeDanglingOverrides() {
	// Property
	for (auto it = prefabPropertyOverrides.begin(); it != prefabPropertyOverrides.end();) {
		IComponentData* component = it->targetComponent.getComponent();
		if (it->targetComponent.isValid() && !component->getType().getProperty(it->targetPropertyName).name.empty()) {
			it++;
		}
		else {
			it = prefabPropertyOverrides.erase(it);
		}
	}
}