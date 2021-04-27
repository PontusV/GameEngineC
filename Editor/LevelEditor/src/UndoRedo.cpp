#include "UndoRedo.h"
#include "EngineDLL.h"
#include <iostream>

using namespace Editor;

/* Swaps the values in propertyFields with the previous ones */
bool setPropertyFields(EngineDLL* engineDLL, EntityID entityID, TypeID typeID, std::string propertyName, std::vector<std::shared_ptr<IPropertyValueData>>& propertyFields) {
	std::size_t propIndex = -1;
	std::size_t propertiesCount = engineDLL->getPropertiesCount(typeID);
	for (std::size_t i = 0; i < propertiesCount; i++) {
		std::string propName = engineDLL->getPropertyName(typeID, i);
		if (propName.compare(propertyName) == 0) {
			propIndex = i;
			break;
		}
	}
	if (propIndex == -1) {
		std::cout << "setPropertyFields::call::ERROR Unable to find property with name" << std::endl; // Note: Probable cause Engine DLL hot reload
		return false;
	}
	ComponentData component = engineDLL->getComponent(entityID, typeID);
	std::vector<ReflectedFieldData> fields = engineDLL->getPropertyFields(typeID, propIndex, component.instance);
	if (fields.size() != propertyFields.size()) {
		std::cout << "setPropertyFields::call::ERROR Field count mismatch" << std::endl; // Note: Probable cause Engine DLL hot reload
		return false;
	}
	for (std::size_t i = 0; i < fields.size(); i++) {
		if (!propertyFields[i]->isTriviallyCopyable() && fields[i].size != propertyFields[i]->getMemSize()) {
			std::cout << "setPropertyFields::call::ERROR Field size mismatch" << std::endl; // Note: Probable cause Engine DLL hot reload
			return false;
		}
	}
	for (std::size_t i = 0; i < fields.size(); i++) {
		propertyFields[i] = propertyFields[i]->assignValueTo(fields[i]);
	}
	engineDLL->onUpdate(component.instance, typeID, propIndex);
	return true;
}

// ---------------------------- End of utility --------------------------------------------

DoAction::~DoAction() {}

bool UndoRedoManager::undo() {
	if (undoStack.empty()) return false;
	if (!engineDLL->isLoaded()) {
		std::cout << "UndoRedoManager::undo::ERROR Unable to undo while no Engine DLL is loaded" << std::endl;
		return false;
	}
	auto nextAction = undoStack.back()->call(engineDLL);
	if (nextAction != nullptr) {
		std::size_t sceneIndex = undoStack.back()->getSceneIndex();
		stepsSinceSave[sceneIndex]--;
		undoStack.pop_back();
		redoStack.push_back(std::move(nextAction));
		return true;
	}
	std::cout << "UndoRedoManager::undo::ERROR Failed to undo" << std::endl;
	clearStack();
	return false;
}
bool UndoRedoManager::redo() {
	if (redoStack.empty()) return false;
	if (!engineDLL->isLoaded()) {
		std::cout << "UndoRedoManager::redo::ERROR Unable to redo while no Engine DLL is loaded" << std::endl;
		return false;
	}
	auto nextAction = redoStack.back()->call(engineDLL);
	if (nextAction != nullptr) {
		std::size_t sceneIndex = redoStack.back()->getSceneIndex();
		stepsSinceSave[sceneIndex]++;
		redoStack.pop_back();
		undoStack.push_back(std::move(nextAction));
		return true;
	}
	std::cout << "UndoRedoManager::redo::ERROR Failed to redo" << std::endl;
	clearStack();
	return false;
}

void UndoRedoManager::clearStack() {
	undoStack.clear();
	redoStack.clear();
}

void UndoRedoManager::removeSceneFromStack(std::size_t sceneIndex) {
	for (auto it = undoStack.begin(); it != undoStack.end();) {
		if (it->get()->getSceneIndex() == sceneIndex) {
			it = undoStack.erase(it);
		}
		else {
			it++;
		}
	}
	for (auto it = redoStack.begin(); it != redoStack.end();) {
		if (it->get()->getSceneIndex() == sceneIndex) {
			it = redoStack.erase(it);
		}
		else {
			it++;
		}
	}
	stepsSinceSave.erase(sceneIndex);
}

void UndoRedoManager::registerUndo(std::unique_ptr<DoAction>&& action) {
	std::size_t sceneIndex = action->getSceneIndex();
	auto it = stepsSinceSave.find(sceneIndex);
	if (it == stepsSinceSave.end()) {
		stepsSinceSave[sceneIndex] = 0;
	}
	undoStack.push_back(std::move(action));
	redoStack.clear();
}

void UndoRedoManager::registerRedo(std::unique_ptr<DoAction>&& action) {
	std::size_t sceneIndex = action->getSceneIndex();
	auto it = stepsSinceSave.find(sceneIndex);
	if (it != stepsSinceSave.end()) {
		stepsSinceSave[sceneIndex] = 0;
	}
	redoStack.push_back(std::move(action));
}

bool UndoRedoManager::isUndoAvailable() {
	if (engineDLL->isLoaded()) {
		return undoStack.size() > 0;
	}
	return false;
}

bool UndoRedoManager::isRedoAvailable() {
	if (engineDLL->isLoaded()) {
		return redoStack.size() > 0;
	}
	return false;
}

void UndoRedoManager::resetStepsSinceSave(std::size_t sceneIndex) {
	stepsSinceSave[sceneIndex] = 0;
}

int UndoRedoManager::getStepsSinceSave(std::size_t sceneIndex) {
	return stepsSinceSave[sceneIndex];
}

// -- Actions --

std::unique_ptr<DoAction> CreateEntityAction::call(EngineDLL* engineDLL) {
	if (EntityID entityID = engineDLL->createEntity(sceneIndex, entityName.c_str())) {
		for (auto& componentBP : blueprint.componentBPs) {
			std::size_t typeID = engineDLL->getTypeIDFromName(componentBP.typeName.c_str());
			if (!engineDLL->addComponent(entityID, sceneIndex, typeID)) {
				std::cout << "CreateEntityAction::call::ERROR Failed to add component. Cleaning up..." << std::endl;
				engineDLL->destroyEntity(sceneIndex, entityID); // Cleanup
				return nullptr;
			}
			auto component = engineDLL->getComponent(entityID, typeID);
			auto properties = engineDLL->getProperties(typeID, component.instance);
			for (auto& propertyBP : componentBP.propertyBPs) {
				if (!setPropertyFields(engineDLL, entityID, typeID, propertyBP.name, propertyBP.propertyValues)) {
					std::cout << "CreateEntityAction::call::ERROR Failed to set property fields" << std::endl;
				}
			}
		}
		return std::make_unique<DestroyEntityAction>(sceneIndex, entityName);
	}
	return nullptr;
}

std::unique_ptr<DoAction> DestroyEntityAction::call(EngineDLL* engineDLL) {
	auto entityID = engineDLL->getEntityFromName(entityName.c_str());
	ImVec2 position = engineDLL->getWorldPosition(entityID);
	ImVec2 size = engineDLL->getRectSize(entityID);
	auto blueprint = EntityBlueprint::createFromEntity(engineDLL, entityID);
	if (engineDLL->destroyEntity(sceneIndex, entityID)) {
		return std::make_unique<CreateEntityAction>(sceneIndex, entityName, std::move(blueprint));
	}
	return nullptr;
}

std::unique_ptr<DoAction> AddComponentAction::call(EngineDLL* engineDLL) {
	auto entityID = engineDLL->getEntityFromName(entityName.c_str());
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	if (engineDLL->addComponent(entityID, sceneIndex, typeID)) {
		auto component = engineDLL->getComponent(entityID, typeID);
		auto properties = engineDLL->getProperties(typeID, component.instance);
		for (auto& propertyBP : blueprint.propertyBPs) {
			if (!setPropertyFields(engineDLL, entityID, typeID, propertyBP.name, propertyBP.propertyValues)) {
				std::cout << "AddComponentAction::call::ERROR Failed to set property fields" << std::endl;
			}
		}
		return std::make_unique<RemoveComponentAction>(sceneIndex, entityName, typeName);
	}
	return nullptr;
}

std::unique_ptr<DoAction> RemoveComponentAction::call(EngineDLL* engineDLL) {
	auto entityID = engineDLL->getEntityFromName(entityName.c_str());
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	ComponentData componentData = engineDLL->getComponent(entityID, typeID);
	ComponentBlueprint blueprint = ComponentBlueprint::createFromComponent(engineDLL, componentData);
	if (engineDLL->removeComponent(entityID, sceneIndex, typeID)) {
		return std::make_unique<AddComponentAction>(sceneIndex, entityName, typeName, std::move(blueprint));
	}
	return nullptr;
}

std::unique_ptr<DoAction> MoveEntityAction::call(EngineDLL* engineDLL) {
	auto entityID = engineDLL->getEntityFromName(entityName.c_str());
	ImVec2 currentPosition = engineDLL->getWorldPosition(entityID);
	if (engineDLL->setWorldPosition(entityID, x, y)) {
		return std::make_unique<MoveEntityAction>(sceneIndex, entityName, currentPosition.x, currentPosition.y);
	}
	return nullptr;
}

std::unique_ptr<DoAction> RenameEntityAction::call(EngineDLL* engineDLL) {
	auto entityID = engineDLL->getEntityFromName(entityName.c_str());
	if (engineDLL->renameEntity(entityID, name.c_str())) {
		return std::make_unique<RenameEntityAction>(sceneIndex, name, entityName);
	}
	return nullptr;
}

std::unique_ptr<DoAction> SetEntityParentAction::call(EngineDLL* engineDLL) {
	auto entityID = engineDLL->getEntityFromName(entityName.c_str());
	auto parentID = engineDLL->getEntityFromName(parentName.c_str());
	auto currentParentID = engineDLL->getEntityParent(entityID);
	auto currentParentName = engineDLL->getEntityName(currentParentID);
	if (engineDLL->setEntityParent(sceneIndex, entityID, parentID)) {
		return std::make_unique<SetEntityParentAction>(sceneIndex, entityName, currentParentName);
	}
	return nullptr;
}

std::unique_ptr<DoAction> PropertyAssignAction::call(EngineDLL* engineDLL) {
	auto entityID = engineDLL->getEntityFromName(entityName.c_str());
	auto typeID = engineDLL->getTypeIDFromName(typeName.c_str());
	if (entityID == 0) {
		std::cout << "PropertyAssignAction::call::ERROR Failed to retrieve EntityID with name " << entityName << std::endl;
		return nullptr;
	}
	if (typeID == 0) {
		std::cout << "PropertyAssignAction::call::ERROR Failed to retrieve TypeID with name " << typeName << std::endl;
		return nullptr;
	}
	
	if (!setPropertyFields(engineDLL, entityID, typeID, propertyName, propertyValues)) {
		std::cout << "PropertyAssignAction::call::ERROR Failed to set property fields" << std::endl;
		return nullptr;
	}
	// TODO: Change GameView target and ImGui focus
	return std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, propertyName, std::move(propertyValues));
}

std::vector<std::shared_ptr<IPropertyValueData>> Editor::createPropData(ReflectedPropertyData& prop) {
	if (prop.renderer == InspectorFieldRenderType::BOOL) {
		bool* ptr = static_cast<bool*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::DECIMAL) { // Note: Decimals are always signed
		ImGuiDataType dataType =
			prop.typeName == "float" ? ImGuiDataType_Float :
			prop.typeName == "double" ? ImGuiDataType_Double :
			prop.typeName == "long double" ? ImGuiDataType_Double :
			ImGuiDataType_Float;
		if (dataType == ImGuiDataType_Float) {
			float* ptr = static_cast<float*>(prop.fields[0].ptr);
			return { createPropValueData(*ptr) };
		}
		else {
			double* ptr = static_cast<double*>(prop.fields[0].ptr);
			return { createPropValueData(*ptr) };
		}
	}
	else if (prop.renderer == InspectorFieldRenderType::SIGNED_CHAR) {
		int8_t* ptr = static_cast<int8_t*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::SIGNED_NUMBER) {
		int32_t* ptr = static_cast<int32_t*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::UNSIGNED_CHAR) {
		uint8_t* ptr = static_cast<uint8_t*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::UNSIGNED_NUMBER) {
		uint32_t* ptr = static_cast<uint32_t*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::STRING) {
		std::string* ptr = static_cast<std::string*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::WIDE_STRING) {
		std::wstring* ptr = static_cast<std::wstring*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::IMAGE_PATH) {
		std::string* ptr = static_cast<std::string*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::SHADER_PATH) {
		std::string* ptr = static_cast<std::string*>(prop.fields[0].ptr);
		return { createPropValueData(*ptr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::VECTOR2) {
		float* xPtr = static_cast<float*>(prop.fields[0].ptr);
		float* yPtr = static_cast<float*>(prop.fields[1].ptr);
		return { createPropValueData(*xPtr), createPropValueData(*yPtr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::COLOR) {
		unsigned char* rPtr = static_cast<unsigned char*>(prop.fields[0].ptr);
		unsigned char* gPtr = static_cast<unsigned char*>(prop.fields[1].ptr);
		unsigned char* bPtr = static_cast<unsigned char*>(prop.fields[2].ptr);
		unsigned char* aPtr = static_cast<unsigned char*>(prop.fields[3].ptr);
		return { createPropValueData(*rPtr), createPropValueData(*gPtr), createPropValueData(*bPtr), createPropValueData(*aPtr) };
	}
	else if (prop.renderer == InspectorFieldRenderType::FONT) {
		std::string* fontPath = static_cast<std::string*>(prop.fields[0].ptr);
		int* fontSize = static_cast<int*>(prop.fields[1].ptr);
		return { createPropValueData(*fontPath), createPropValueData(*fontSize) };
	}
	else {
		std::cout << "createFromProperty::ERROR Unknown property type" << std::endl;
		throw "createFromProperty::ERROR Unknown property type";
	}
	return {};
}

EntityBlueprint EntityBlueprint::createFromEntity(EngineDLL* engineDLL, EntityID entityID) {
	EntityBlueprint instance;
	std::vector<ComponentData> components = engineDLL->getComponents(entityID);
	instance.componentBPs.resize(components.size());
	for (std::size_t i = 0; i < components.size(); i++) {
		ComponentData& componentData = components[i];
		ComponentBlueprint& componentBP = instance.componentBPs[i];
		componentBP = ComponentBlueprint::createFromComponent(engineDLL, componentData);
	}
	return instance;
}

ComponentBlueprint ComponentBlueprint::createFromComponent(EngineDLL* engineDLL, ComponentData componentData) {
	ComponentBlueprint blueprint;
	blueprint.typeName = engineDLL->getTypeName(componentData.typeID);
	std::vector<ReflectedPropertyData> properties = engineDLL->getProperties(componentData.typeID, componentData.instance);
	blueprint.propertyBPs.resize(properties.size());
	for (std::size_t propIndex = 0; propIndex < properties.size(); propIndex++) {
		blueprint.propertyBPs[propIndex].propertyValues = createPropData(properties[propIndex]);
		blueprint.propertyBPs[propIndex].name = properties[propIndex].name;
		blueprint.propertyBPs[propIndex].typeName = properties[propIndex].typeName;
	}
	return blueprint;
}