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

bool PrefabComponent::setDirty(bool value) {
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

std::vector<PrefabPropertyOverride> PrefabComponent::getPropertyOverrides() const {
	return prefabPropertyOverrides;
}

bool PrefabComponent::addComponentOverride(PrefabComponentOverride prefabComponentOverride) {
	// Only allows a component to be overriden once
	auto is_equal = [&prefabComponentOverride](PrefabComponentOverride& override) { return override.targetEntity == prefabComponentOverride.targetEntity && override.typeID == prefabComponentOverride.typeID; };
	if (std::find_if(prefabComponentOverrides.begin(), prefabComponentOverrides.end(), is_equal) != prefabComponentOverrides.end()) {
		//std::cout << "PrefabComponent::addComponentOverride::ERROR A matching override already exists" << std::endl;
		return false;
	}
	prefabComponentOverrides.push_back(prefabComponentOverride);
	return true;
}

bool PrefabComponent::removeComponentOverride(PrefabComponentOverride prefabComponentOverride) {
	auto is_equal = [&prefabComponentOverride](PrefabComponentOverride& override) { return override.targetEntity == prefabComponentOverride.targetEntity && override.typeID == prefabComponentOverride.typeID; };
	auto it = std::find_if(prefabComponentOverrides.begin(), prefabComponentOverrides.end(), is_equal);
	if (it != prefabComponentOverrides.end()) {
		prefabComponentOverrides.erase(it);
		return true;
	}
	return false;
}

std::vector<PrefabComponentOverride> PrefabComponent::getComponentOverrides() const {
	return prefabComponentOverrides;
}

void PrefabComponent::setEntityRemapInfo(std::vector<std::pair<std::string, std::size_t>> entityRemapInfo) {
	std::vector<std::pair<std::string, Entity>> newValue;
	for (const auto& pair : entityRemapInfo) {
		newValue.push_back(std::pair<std::string, Entity>(pair.first, Entity(pair.second)));
	}
	this->entityRemapInfo = newValue;
}

std::vector<std::pair<std::string, std::size_t>> PrefabComponent::getEntityRemapInfo() const {
	std::vector<std::pair<std::string, std::size_t>> result;
	for (const auto& pair : entityRemapInfo) {
		result.push_back(std::pair<std::string, std::size_t>(pair.first, pair.second.getID()));
	}
	return result;
}

void PrefabComponent::removeDanglingOverrides() {
	auto& propertyOverrides = prefabPropertyOverrides;
	auto& componentOverrides = prefabComponentOverrides;
	// Property
	for (auto it = propertyOverrides.begin(); it != propertyOverrides.end();) {
		IComponentData* component = it->targetComponent.getComponent();
		if (it->targetComponent.isValid() && !component->getType().getProperty(it->targetPropertyName).name.empty()) {
			it++;
		}
		else {
			it = propertyOverrides.erase(it);
		}
	}
	// Component
	for (auto it = componentOverrides.begin(); it != componentOverrides.end();) {
		if (it->targetEntity.refresh() && it->targetEntity.hasComponent(it->typeID)) {
			it++;
		}
		else {
			it = componentOverrides.erase(it);
		}
	}
}