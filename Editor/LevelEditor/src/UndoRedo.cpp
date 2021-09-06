#include "UndoRedo.h"
#include "EngineDLL.h"
#include <iostream>

using namespace Editor;

bool entityExists(EngineDLL* engineDLL, std::size_t entityID) {
	auto entities = engineDLL->getAllEntities();
	return std::find(entities.begin(), entities.end(), entityID) != entities.end();
}

DoAction::~DoAction() {}

bool UndoRedoManager::undo() {
	if (!enabled || undoStack.empty()) return false;
	if (!engineDLL->isLoaded()) {
		std::cout << "UndoRedoManager::undo::ERROR Unable to undo while no Engine DLL is loaded" << std::endl;
		return false;
	}
	while (!undoStack.empty() && !undoStack.back()->isValid(engineDLL)) {
		undoStack.pop_back();
	}
	if (undoStack.empty()) {
		return false;
	}
	auto nextAction = undoStack.back()->call(engineDLL);
	if (nextAction != nullptr) {
		std::size_t rootEntityID = undoStack.back()->getRootEntityID();
		stepsSinceSave[rootEntityID]--;
		undoStack.pop_back();
		redoStack.push_back(std::move(nextAction));
		return true;
	}
	std::cout << "UndoRedoManager::undo::ERROR Failed to undo" << std::endl;
	clearStack();
	return false;
}

bool UndoRedoManager::redo() {
	if (!enabled || redoStack.empty()) return false;
	if (!engineDLL->isLoaded()) {
		std::cout << "UndoRedoManager::redo::ERROR Unable to redo while no Engine DLL is loaded" << std::endl;
		return false;
	}
	while (!redoStack.empty() && !redoStack.back()->isValid(engineDLL)) {
		redoStack.pop_back();
	}
	if (redoStack.empty()) {
		return false;
	}
	auto nextAction = redoStack.back()->call(engineDLL);
	if (nextAction != nullptr) {
		std::size_t rootEntityID = redoStack.back()->getRootEntityID();
		stepsSinceSave[rootEntityID]++;
		redoStack.pop_back();
		undoStack.push_back(std::move(nextAction));
		return true;
	}
	std::cout << "UndoRedoManager::redo::ERROR Failed to redo" << std::endl;
	clearStack();
	return false;
}

void UndoRedoManager::enable() {
	enabled = true;
}

void UndoRedoManager::disable() {
	enabled = false;
}

void UndoRedoManager::clearStack() {
	undoStack.clear();
	redoStack.clear();
}

void UndoRedoManager::removeEntityFromStack(std::size_t rootEntityID) {
	for (auto it = undoStack.begin(); it != undoStack.end();) {
		if (it->get()->getRootEntityID() == rootEntityID) {
			it = undoStack.erase(it);
		}
		else {
			it++;
		}
	}
	for (auto it = redoStack.begin(); it != redoStack.end();) {
		if (it->get()->getRootEntityID() == rootEntityID) {
			it = redoStack.erase(it);
		}
		else {
			it++;
		}
	}
	stepsSinceSave.erase(rootEntityID);
}

void UndoRedoManager::registerUndo(std::unique_ptr<DoAction>&& action) {
	if (!enabled) return;
	std::size_t rootEntityID = action->getRootEntityID();
	auto it = stepsSinceSave.find(rootEntityID);
	if (it == stepsSinceSave.end()) {
		stepsSinceSave[rootEntityID] = 0;
	}
	else {
		it->second++;
	}
	undoStack.push_back(std::move(action));
	redoStack.clear();
}

void UndoRedoManager::registerRedo(std::unique_ptr<DoAction>&& action) {
	if (!enabled) return;
	std::size_t rootEntityID = action->getRootEntityID();
	auto it = stepsSinceSave.find(rootEntityID);
	if (it != stepsSinceSave.end()) {
		stepsSinceSave[rootEntityID] = 0;
	}
	else {
		it->second--;
	}
	redoStack.push_back(std::move(action));
}

bool UndoRedoManager::isUndoAvailable() {
	if (!enabled) return false;
	if (engineDLL->isLoaded()) {
		return undoStack.size() > 0;
	}
	return false;
}

bool UndoRedoManager::isRedoAvailable() {
	if (!enabled) return false;
	if (engineDLL->isLoaded()) {
		return redoStack.size() > 0;
	}
	return false;
}

void UndoRedoManager::resetStepsSinceSave(std::size_t rootEntityID) {
	stepsSinceSave[rootEntityID] = 0;
}

int UndoRedoManager::getStepsSinceSave(std::size_t rootEntityID) {
	return stepsSinceSave[rootEntityID];
}

bool UndoRedoManager::isUnsaved(std::size_t rootEntityID) {
	return getStepsSinceSave(rootEntityID) != 0 || std::find(dirtyRootEntityIDs.begin(), dirtyRootEntityIDs.end(), rootEntityID) != dirtyRootEntityIDs.end();
}

void UndoRedoManager::setDirty(std::size_t rootEntityID, bool value) {
	auto it = std::find(dirtyRootEntityIDs.begin(), dirtyRootEntityIDs.end(), rootEntityID);
	if (value && it == dirtyRootEntityIDs.end()) {
		dirtyRootEntityIDs.push_back(rootEntityID);
	}
	else if (!value && it != dirtyRootEntityIDs.end()) {
		dirtyRootEntityIDs.erase(it);
	}
}

// -- Actions --

std::unique_ptr<DoAction> CreateEntityAction::call(EngineDLL* engineDLL) {
	if (engineDLL->loadEntityFromBuffer(serializedEntityData.c_str(), serializedEntityData.size())) {
		return std::make_unique<DestroyEntityAction>(rootEntityID, entityID);
	}
	return nullptr;
}

bool CreateEntityAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && !serializedEntityData.empty() && entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> DestroyEntityAction::call(EngineDLL* engineDLL) {
	std::string serializedEntityData = engineDLL->writeEntityToBuffer(entityID);
	if (serializedEntityData.empty()) {
		std::cout << "DestroyEntityAction::call::ERROR Failed to retrieve serialized entity data" << std::endl;
		return nullptr;
	}
	if (engineDLL->destroyEntity(entityID)) {
		return std::make_unique<CreateEntityAction>(rootEntityID, entityID, std::move(serializedEntityData));
	}
	return nullptr;
}

bool DestroyEntityAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> AddComponentAction::call(EngineDLL* engineDLL) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	if (typeID == 0) {
		std::cout << "AddComponentAction::call::ERROR Failed to retrieve TypeID with name " << typeName << std::endl;
		return nullptr;
	}
	auto componentOverrides = engineDLL->getComponentOverrides(entityID);
	bool overriden = std::find_if(componentOverrides.begin(), componentOverrides.end(), [&typeID](const ComponentOverride& override) { return override.typeID == typeID; }) != componentOverrides.end();
	if (!prevOverriden) {
		if (prevOverriden && !engineDLL->removeComponentOverride(entityID, typeID)) {
			std::cout << "AddComponentAction::call::ERROR Failed to remove component override" << std::endl;
			return nullptr;
		}
		EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
		if (!engineDLL->updatePrefab(rootPrefabEntityID)) {
			std::cout << "AddComponentAction::call::ERROR Failed to update prefab" << std::endl;
			return nullptr;
		}
		// TODO: Set rootEntityID as dirty
		return std::make_unique<RemoveComponentAction>(rootEntityID, entityID, typeName, overriden);
	} else if (engineDLL->loadComponentFromBuffer(entityID, serializedComponentData.c_str(), serializedComponentData.size())) {
		if (!overriden && !engineDLL->overrideComponent(entityID, typeID)) {
			std::cout << "AddComponentAction::call::ERROR Failed to override component" << std::endl;
			return nullptr;
		}
		return std::make_unique<RemoveComponentAction>(rootEntityID, entityID, typeName, overriden);
	}
	return nullptr;
}

bool AddComponentAction::isValid(EngineDLL* engineDLL) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	return entityID != 0 && typeID != 0 && !serializedComponentData.empty() && entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> RemoveComponentAction::call(EngineDLL* engineDLL) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	if (typeID == 0) {
		std::cout << "RemoveComponentAction::call::ERROR Failed to retrieve TypeID with name " << typeName << std::endl;
		return nullptr;
	}
	std::string serializedComponentData = engineDLL->writeComponentToBuffer(entityID, typeID);
	auto componentOverrides = engineDLL->getComponentOverrides(entityID);
	bool overriden = std::find_if(componentOverrides.begin(), componentOverrides.end(), [&typeID](const ComponentOverride & override) { return override.typeID == typeID; }) != componentOverrides.end();
	if (serializedComponentData.empty()) {
		std::cout << "RemoveComponentAction::call::ERROR Failed to retrieve serialized component data" << std::endl;
		return nullptr;
	}
	if (!prevOverriden) {
		if (prevOverriden && !engineDLL->removeComponentOverride(entityID, typeID)) {
			std::cout << "RemoveComponentAction::call::ERROR Failed to remove component override" << std::endl;
			return nullptr;
		}
		EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
		if (!engineDLL->updatePrefab(rootPrefabEntityID)) {
			std::cout << "RemoveComponentAction::call::ERROR Failed to update prefab" << std::endl;
			return nullptr;
		}
		// TODO: Set rootEntityID as dirty
		return std::make_unique<AddComponentAction>(rootEntityID, entityID, typeName, std::move(serializedComponentData), overriden);
	}
	else if (engineDLL->removeComponent(entityID, typeID)) {
		if (!overriden && !engineDLL->overrideComponent(entityID, typeID)) {
			std::cout << "RemoveComponentAction::call::ERROR Failed to override component" << std::endl;
			return nullptr;
		}
		return std::make_unique<AddComponentAction>(rootEntityID, entityID, typeName, std::move(serializedComponentData), overriden);
	}
	return nullptr;
}

bool RemoveComponentAction::isValid(EngineDLL* engineDLL) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	return entityID != 0 && typeID != 0 && entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> MoveEntityAction::call(EngineDLL* engineDLL) {
	ImVec2 currentPosition = engineDLL->getWorldPosition(entityID);
	if (engineDLL->setWorldPosition(entityID, x, y)) {
		return std::make_unique<MoveEntityAction>(rootEntityID, entityID, currentPosition.x, currentPosition.y);
	}
	return nullptr;
}

bool MoveEntityAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && entityExists(engineDLL, entityID); // TODO: Check if entity has transform (low prio)
}

std::unique_ptr<DoAction> RenameEntityAction::call(EngineDLL* engineDLL) {
	if (engineDLL->setEntityName(entityID, name.c_str())) {
		return std::make_unique<RenameEntityAction>(rootEntityID, entityID, name);
	}
	return nullptr;
}

bool RenameEntityAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && entityExists(engineDLL, entityID); // TODO: Check if entity has name (low prio)
}

std::unique_ptr<DoAction> SetEntityParentAction::call(EngineDLL* engineDLL) {
	auto currentParentEntityID = engineDLL->getEntityParent(entityID);
	if (engineDLL->setEntityParent(entityID, parentEntityID)) {
		return std::make_unique<SetEntityParentAction>(rootEntityID, entityID, currentParentEntityID);
	}
	return nullptr;
}

bool SetEntityParentAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && parentEntityID == 0 && entityExists(engineDLL, entityID) || entityID != 0 && parentEntityID != 0 && entityExists(engineDLL, entityID) && entityExists(engineDLL, parentEntityID);
}

std::unique_ptr<DoAction> PropertyAssignAction::call(EngineDLL* engineDLL) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	if (typeID == 0) {
		std::cout << "PropertyAssignAction::call::ERROR Failed to retrieve TypeID with name " << typeName << std::endl;
		return nullptr;
	}
	std::size_t propIndex = engineDLL->getPropertyIndex(typeID, propertyName.c_str());
	if (propIndex == -1) {
		std::cout << "PropertyAssignAction::call::ERROR Failed to retrieve property index with name " << propertyName << std::endl;
		return nullptr;
	}
	std::string prevSerializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, propIndex);
	auto propertyOverrides = engineDLL->getPropertyOverrides(entityID, typeID);
	bool overriden = std::find_if(propertyOverrides.begin(), propertyOverrides.end(), [&](const PropertyOverride& override) { return override.propertyName == propertyName; }) != propertyOverrides.end();
	if (prevSerializedPropertyData.empty()) {
		std::cout << "PropertyAssignAction::call::ERROR Failed to retrieve serialized property data" << std::endl;
		return nullptr;
	}
	if (!prevOverriden) {
		if (overriden && !engineDLL->removePropertyOverride(entityID, typeID, propIndex)) {
			std::cout << "PropertyAssignAction::call::ERROR Failed to remove property override" << std::endl;
		}
		EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
		if (!engineDLL->updatePrefab(rootPrefabEntityID)) {
			std::cout << "PropertyAssignAction::call::ERROR Failed to update prefab" << std::endl;
		}
		// TODO: Set rootEntityID as dirty
		auto component = engineDLL->getComponent(entityID, typeID);
		engineDLL->onUpdate(component.instance, typeID, propIndex);
		return std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, propertyName, std::move(prevSerializedPropertyData), overriden);
	} else if (engineDLL->loadPropertyFromBuffer(entityID, typeID, serializedPropertyData.c_str(), serializedPropertyData.size())) {
		if (!overriden && !engineDLL->overrideProperty(entityID, typeID, propIndex)) {
			std::cout << "PropertyAssignAction::call::ERROR Failed to override property" << std::endl;
			return nullptr;
		}
		auto component = engineDLL->getComponent(entityID, typeID);
		engineDLL->onUpdate(component.instance, typeID, propIndex);
		return std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, propertyName, std::move(prevSerializedPropertyData), overriden);
	}
	return nullptr;
}

bool PropertyAssignAction::isValid(EngineDLL* engineDLL) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	auto propIndex = engineDLL->getPropertyIndex(typeID, propertyName.c_str());
	return entityID != 0 && typeID != 0 && propIndex != -1 && !serializedPropertyData.empty() && entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> UnpackPrefabAction::call(EngineDLL* engineDLL) {
	auto propertyOverrides = engineDLL->getPropertyOverridesAt(entityID);
	auto prefabTypeID = engineDLL->getPrefabComponentTypeID();
	auto componentOverrides = engineDLL->getComponentOverridesAt(entityID);
	auto serializedComponentData = engineDLL->writeComponentToBuffer(entityID, prefabTypeID);
	if (engineDLL->unpackPrefab(entityID)) {
		return std::make_unique<RepackPrefabAction>(rootEntityID, entityID, std::move(serializedComponentData), propertyOverrides, componentOverrides);
	}
	return nullptr;
}

bool UnpackPrefabAction::isValid(EngineDLL* engineDLL) {
	auto isPrefabRoot = engineDLL->isEntityPrefabRoot(entityID);
	return entityID != 0 && isPrefabRoot; // Note: isPrefabRoot will be false if entityID does not exist
}

std::unique_ptr<DoAction> RepackPrefabAction::call(EngineDLL* engineDLL) {
	if (!engineDLL->loadComponentFromBuffer(entityID, serializedComponentData.c_str(), serializedComponentData.size())) {
		std::cout << "RepackPrefabAction::call::ERROR Failed to load component from buffer" << std::endl;
		return nullptr;
	}
	for (auto& propertyOverride : propertyOverrides) {
		std::size_t propIndex = engineDLL->getPropertyIndex(propertyOverride.typeID, propertyOverride.propertyName.c_str());
		if (propIndex == -1) {
			std::cout << "RepackPrefabAction::call::WARNING Failed to override property " << propertyOverride.propertyName << ". Unable to find property index" << std::endl;
			continue;
		}
		if (!engineDLL->overrideProperty(propertyOverride.entityID, propertyOverride.typeID, propIndex)) {
			std::cout << "RepackPrefabAction::call::WARNING Failed to override property " << propertyOverride.propertyName << std::endl;
		}
	}
	for (auto& componentOverride : componentOverrides) {
		if (!engineDLL->overrideComponent(componentOverride.entityID, componentOverride.typeID)) {
			std::cout << "RepackPrefabAction::call::WARNING Failed to override component " << componentOverride.typeID << std::endl;
		}
	}
	EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
	engineDLL->updatePrefab(rootPrefabEntityID);
	// TODO: Set rootEntityID as dirty
	return std::make_unique<UnpackPrefabAction>(rootEntityID, entityID);
}

bool RepackPrefabAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && !serializedComponentData.empty() && entityExists(engineDLL, entityID); // Note: No need to check if overrides are valid. Invalid overrides should only be skipped and not stop the whole action.
}

std::unique_ptr<DoAction> RevertPrefabAction::call(EngineDLL* engineDLL) {
	auto propertyOverrides = engineDLL->getPropertyOverridesAt(entityID);
	std::vector<SerializedProperty> serializedProperties;
	serializedProperties.reserve(propertyOverrides.size());
	for (auto& propertyOverride : propertyOverrides) {
		std::size_t propertyIndex = engineDLL->getPropertyIndex(propertyOverride.typeID, propertyOverride.propertyName.c_str());
		SerializedProperty& serializedProperty = serializedProperties.emplace_back();
		serializedProperty.entityID = propertyOverride.entityID;
		serializedProperty.typeName = engineDLL->getTypeName(propertyOverride.typeID);
		serializedProperty.propertyName = propertyOverride.propertyName;
		serializedProperty.serializedData = engineDLL->writePropertyToBuffer(propertyOverride.entityID, propertyOverride.typeID, propertyIndex);
	}
	auto componentOverrides = engineDLL->getComponentOverridesAt(entityID);
	std::vector<SerializedComponent> serializedComponents(componentOverrides.size());
	serializedComponents.reserve(componentOverrides.size());
	for (auto& componentOverride : componentOverrides) {
		SerializedComponent& serializedComponent = serializedComponents.emplace_back();
		serializedComponent.entityID = componentOverride.entityID;
		serializedComponent.typeName = engineDLL->getTypeName(componentOverride.typeID);
		serializedComponent.serializedData = engineDLL->writeComponentToBuffer(componentOverride.entityID, componentOverride.typeID);
	}
	if (engineDLL->clearOverrides(entityID)) {
		EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
		if (!engineDLL->updatePrefab(rootPrefabEntityID)) {
			std::cout << "ReverPrefabAction::ERROR Failed to update prefab after revert" << std::endl;
		}
		// TODO: Set rootEntityID as dirty
		return std::make_unique<UndoRevertPrefabAction>(rootEntityID, entityID, serializedProperties, serializedComponents);
	}
	return nullptr;
}

bool RevertPrefabAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> UndoRevertPrefabAction::call(EngineDLL* engineDLL) {
	for (auto& serializedProperty : serializedProperties) {
		std::size_t typeID = engineDLL->getTypeIDFromName(serializedProperty.typeName.c_str());
		std::size_t propIndex = engineDLL->getPropertyIndex(typeID, serializedProperty.propertyName.c_str());
		if (propIndex == -1) {
			std::cout << "UndoRevertPrefabAction::call::WARNING Failed to override property " << serializedProperty.propertyName << ". Unable to find property index" << std::endl;
			continue;
		}
		if (!engineDLL->overrideProperty(serializedProperty.entityID, typeID, propIndex)) {
			std::cout << "UndoRevertPrefabAction::call::WARNING Failed to override property " << serializedProperty.propertyName << std::endl;
		}
		if (!engineDLL->loadPropertyFromBuffer(serializedProperty.entityID, typeID, serializedProperty.serializedData.c_str(), serializedProperty.serializedData.size())) {
			std::cout << "UndoRevertPrefabAction::call::WARNING Failed to load property " << serializedProperty.propertyName << std::endl;
		}
	}
	for (auto& serializedComponent : serializedComponents) {
		std::size_t typeID = engineDLL->getTypeIDFromName(serializedComponent.typeName.c_str());
		if (!engineDLL->overrideComponent(serializedComponent.entityID, typeID)) {
			std::cout << "UndoRevertPrefabAction::call::WARNING Failed to override component " << serializedComponent.typeName << std::endl;
		}
		if (!engineDLL->loadComponentFromBuffer(serializedComponent.entityID, serializedComponent.serializedData.c_str(), serializedComponent.serializedData.size())) {
			std::cout << "UndoRevertPrefabAction::call::WARNING Failed to load component " << serializedComponent.typeName << std::endl;
		}
	}
	EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
	if (!engineDLL->updatePrefab(rootPrefabEntityID)) {
		std::cout << "UndoRevertPrefabAction::call::ERROR Failed to update prefab after undoing revert" << std::endl;
	}
	// TODO: Set rootEntityID as dirty
	return std::make_unique<RevertPrefabAction>(rootEntityID, entityID);
}

bool UndoRevertPrefabAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && entityExists(engineDLL, entityID);
}