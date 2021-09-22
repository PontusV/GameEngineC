#include "UndoRedo.h"
#include "EngineDLL.h"
#include "windows/Hierarchy.h"
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
	auto nextAction = undoStack.back()->call(engineDLL, this);
	if (nextAction != nullptr) {
		std::size_t rootEntityID = undoStack.back()->getRootEntityID();
		stepsSinceSave[rootEntityID]--;
		undoStack.pop_back();
		redoStack.push_back(std::move(nextAction));
		hierarchy->setDirty(true);
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
	auto nextAction = redoStack.back()->call(engineDLL, this);
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
	if (engineDLL->hasEntityParent(rootEntityID) || !engineDLL->isEntityPrefabRoot(rootEntityID)) {
		std::cout << "UndoRedoManager::setDirty::ERROR Given entityID is not of a root Entity" << std::endl;
		return;
	}
	auto it = std::find(dirtyRootEntityIDs.begin(), dirtyRootEntityIDs.end(), rootEntityID);
	if (value && it == dirtyRootEntityIDs.end()) {
		dirtyRootEntityIDs.push_back(rootEntityID);
	}
	else if (!value && it != dirtyRootEntityIDs.end()) {
		dirtyRootEntityIDs.erase(it);
	}
}

void UndoRedoManager::setAllAsDirty() {
	std::vector<EntityID> entityIDs = engineDLL->getAllEntities();
	std::vector<EntityID> rootEntityIDs;
	for (auto& entityID : entityIDs) {
		if (!engineDLL->hasEntityParent(entityID) && engineDLL->isEntityPrefabRoot(entityID)) {
			auto it = std::find(dirtyRootEntityIDs.begin(), dirtyRootEntityIDs.end(), entityID);
			if (it == dirtyRootEntityIDs.end()) {
				dirtyRootEntityIDs.push_back(entityID);
			}
		}
	}
}

// -- Actions --

std::unique_ptr<DoAction> CreateEntityAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
	if (engineDLL->loadEntityFromBuffer(serializedEntityData.c_str(), serializedEntityData.size())) {
		std::size_t parentID = engineDLL->getEntityParent(entityID);
		if (parentID != 0) {
			if (engineDLL->setEntityParent(entityID, parentID)) {
				engineDLL->overrideParentComponent(entityID);
			}
		}
		return std::make_unique<DestroyEntityAction>(rootEntityID, entityID);
	}
	else {
		std::cout << "CreateEntityAction::call::ERROR Failed to load entity from buffer" << std::endl;
	}
	return nullptr;
}

bool CreateEntityAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && !serializedEntityData.empty() && !entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> DestroyEntityAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
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

std::unique_ptr<DoAction> AddComponentAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	if (typeID == 0) {
		std::cout << "AddComponentAction::call::ERROR Failed to retrieve TypeID with name " << typeName << std::endl;
		return nullptr;
	}
	if (engineDLL->loadComponentFromBuffer(entityID, serializedComponentData.c_str(), serializedComponentData.size())) {
		for (auto& propertyOverride : propertyOverrides) {
			std::size_t propertyIndex = engineDLL->getPropertyIndex(propertyOverride.typeID, propertyOverride.propertyName.c_str());
			if (!engineDLL->overrideProperty(propertyOverride.entityID, propertyOverride.typeID, propertyIndex)) {
				std::cout << "AddComponentAction::call::ERROR Failed to override property. EntityID: " << propertyOverride.entityID << ", TypeID: " << propertyOverride.typeID << ", propertyIndex: " << propertyIndex << std::endl;
			}
		}
		return std::make_unique<RemoveComponentAction>(rootEntityID, entityID, typeName);
	}
	std::cout << "AddComponentAction::call::ERROR Failed to load component from buffer" << std::endl;
	return nullptr;
}

bool AddComponentAction::isValid(EngineDLL* engineDLL) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	return entityID != 0 && typeID != 0 && !serializedComponentData.empty() && entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> RemoveComponentAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	if (typeID == 0) {
		std::cout << "RemoveComponentAction::call::ERROR Failed to retrieve TypeID with name " << typeName << std::endl;
		return nullptr;
	}
	std::string serializedComponentData = engineDLL->writeComponentToBuffer(entityID, typeID);
	if (serializedComponentData.empty()) {
		std::cout << "RemoveComponentAction::call::ERROR Failed to retrieve serialized component data" << std::endl;
		return nullptr;
	}
	auto propertyOverrides = engineDLL->getPropertyOverrides(entityID, typeID);
	if (engineDLL->removeComponent(entityID, typeID)) {
		engineDLL->clearPropertyOverrides(entityID, typeID);
		return std::make_unique<AddComponentAction>(rootEntityID, entityID, typeName, std::move(serializedComponentData), propertyOverrides);
	}
	std::cout << "RemoveComponentAction::call::ERROR Failed to remove component" << std::endl;
	return nullptr;
}

bool RemoveComponentAction::isValid(EngineDLL* engineDLL) {
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	return entityID != 0 && typeID != 0 && entityExists(engineDLL, entityID);
}

std::unique_ptr<DoAction> MoveEntityAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
	ImVec2 currentPosition = engineDLL->getWorldPosition(entityID);
	if (engineDLL->setWorldPosition(entityID, x, y)) {
		bool overriden = engineDLL->isPositionOverriden(entityID);
		if (!prevOverriden) {
			if (overriden && !engineDLL->removePositionOverride(entityID)) {
				std::cout << "MoveEntityAction::call::ERROR Failed to remove property override" << std::endl;
				return nullptr;
			}
			EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
			if (!engineDLL->updatePrefab(rootPrefabEntityID)) {
				std::cout << "MoveEntityAction::call::ERROR Failed to update prefab" << std::endl;
			}
		}
		else {
			if (!overriden && !engineDLL->overridePosition(entityID)) {
				std::cout << "MoveEntityAction::call::ERROR Failed to override property" << std::endl;
				return nullptr;
			}
		}
		return std::make_unique<MoveEntityAction>(rootEntityID, entityID, currentPosition.x, currentPosition.y, overriden);
	}
	return nullptr;
}

bool MoveEntityAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && entityExists(engineDLL, entityID); // TODO: Check if entity has transform (low prio)
}

std::unique_ptr<DoAction> RenameEntityAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
	if (engineDLL->setEntityName(entityID, name.c_str())) {
		return std::make_unique<RenameEntityAction>(rootEntityID, entityID, name);
	}
	return nullptr;
}

bool RenameEntityAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && entityExists(engineDLL, entityID); // TODO: Check if entity has name (low prio)
}

std::unique_ptr<DoAction> SetEntityParentAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
	auto currentParentEntityID = engineDLL->getEntityParent(entityID);
	bool overriden = engineDLL->isParentComponentOverriden(entityID);
	bool positionOverriden = engineDLL->isPositionOverriden(entityID);
	if (prevPositionOverriden && !positionOverriden && !engineDLL->overridePosition(entityID)) {
		std::cout << "SetEntityParentAction::call::ERROR Failed to overriden position before set parent" << std::endl;
	}
	if (!prevPositionOverriden && positionOverriden && !engineDLL->removePositionOverride(entityID)) {
		std::cout << "SetEntityParentAction::call::ERROR Failed to remove position override before set parent" << std::endl;
	}
	if (!prevOverriden) {
		if (overriden && !engineDLL->removeParentComponentOverride(entityID)) {
			std::cout << "SetEntityParentAction::call::ERROR Failed to remove parent component override" << std::endl;
			return nullptr;
		}
		EntityID prefabRootEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
		if (!engineDLL->updatePrefab(prefabRootEntityID)) {
			std::cout << "PropertyAssignAction::call::ERROR Failed to update prefab" << std::endl;
		}
		return std::make_unique<SetEntityParentAction>(rootEntityID, entityID, currentParentEntityID, positionOverriden, overriden);
	} else if (engineDLL->setEntityParent(entityID, parentEntityID)) {
		if (!overriden && !engineDLL->overrideParentComponent(entityID)) {
			std::cout << "SetEntityParentAction::call::ERROR Failed to override parent component" << std::endl;
			return nullptr;
		}
		EntityID prefabRootEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
		if (!engineDLL->updatePrefab(prefabRootEntityID)) {
			std::cout << "PropertyAssignAction::call::ERROR Failed to update prefab" << std::endl;
		}
		return std::make_unique<SetEntityParentAction>(rootEntityID, entityID, currentParentEntityID, positionOverriden, overriden);
	}
	return nullptr;
}

bool SetEntityParentAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && parentEntityID == 0 && entityExists(engineDLL, entityID) || entityID != 0 && parentEntityID != 0 && entityExists(engineDLL, entityID) && entityExists(engineDLL, parentEntityID);
}

std::unique_ptr<DoAction> PropertyAssignAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
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
			return nullptr;
		}
		EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
		if (!engineDLL->updatePrefab(rootPrefabEntityID)) {
			std::cout << "PropertyAssignAction::call::ERROR Failed to update prefab" << std::endl;
		}
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

std::unique_ptr<DoAction> UnpackPrefabAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
	auto propertyOverrides = engineDLL->getPropertyOverridesAt(entityID);
	auto prefabTypeID = engineDLL->getPrefabComponentTypeID();
	auto serializedComponentData = engineDLL->writeComponentToBuffer(entityID, prefabTypeID);
	if (engineDLL->unpackPrefab(entityID)) {
		return std::make_unique<RepackPrefabAction>(rootEntityID, entityID, std::move(serializedComponentData), propertyOverrides);
	}
	return nullptr;
}

bool UnpackPrefabAction::isValid(EngineDLL* engineDLL) {
	auto isPrefabRoot = engineDLL->isEntityPrefabRoot(entityID);
	return entityID != 0 && isPrefabRoot; // Note: isPrefabRoot will be false if entityID does not exist
}

std::unique_ptr<DoAction> RepackPrefabAction::call(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager) {
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
	EntityID rootPrefabEntityID = engineDLL->getNearestPrefabRootEntityID(entityID);
	engineDLL->updatePrefab(rootPrefabEntityID);
	return std::make_unique<UnpackPrefabAction>(rootEntityID, entityID);
}

bool RepackPrefabAction::isValid(EngineDLL* engineDLL) {
	return entityID != 0 && !serializedComponentData.empty() && entityExists(engineDLL, entityID); // Note: No need to check if overrides are valid. Invalid overrides should only be skipped and not stop the whole action.
}