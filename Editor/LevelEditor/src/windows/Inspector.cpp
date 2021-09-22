#include "Inspector.h"
#include "GameView.h"
#include "UndoRedo.h"
#include "EngineDLL.h"
#include "imgui/imgui.h"
#include "ui/PopupManager.h"
#include "utils/file.h"
#include "utils/string.h"
#include <algorithm>
#include <vector>
#include <GLFW/glfw3.h>

#undef min
using namespace Editor;

//-------------

template <typename T>
bool vectorIncludes(std::vector<T> vec, T value) {
	for (const T& e : vec) {
		if (e == value) return true;
	}
	return false;
}

//-------------

Inspector::Inspector(EngineDLL* engineDLL, GameView* gameView, UndoRedoManager* undoRedoManager, PopupManager* popupManager) : engineDLL(engineDLL), gameView(gameView), undoRedoManager(undoRedoManager), popupManager(popupManager){}
Inspector::~Inspector() {}

void Inspector::renderComponent(EntityID rootEntityID, EntityID entityID, std::string entityName, std::size_t typeID, void* component, std::size_t index) {
	if (engineDLL->hasAnnotation(typeID, "hideInInspector")) return;
	std::string typeName = engineDLL->getTypeName(typeID);

	bool closable_group = true;
	if (ImGui::CollapsingHeader(typeName.c_str(), &closable_group, ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();
		for (auto& prop : engineDLL->getProperties(entityID, typeID, component)) {

			if (prop.typeName == "Core::Color") {
				std::string labelString = std::string(prop.name) + "##" + std::to_string(index);
				const char* label = labelString.c_str();
				assert(prop.fields.size() > 0 && prop.fields.size() % 4 == 0);
				for (std::size_t i = 0; i < prop.arraySize; i += 4) {
					assert(prop.fields[i + 0].typeSize == sizeof(unsigned char));
					assert(prop.fields[i + 1].typeSize == sizeof(unsigned char));
					assert(prop.fields[i + 2].typeSize == sizeof(unsigned char));
					assert(prop.fields[i + 3].typeSize == sizeof(unsigned char));
					const unsigned char* rPtr = static_cast<const unsigned char*>(prop.fields[i + 0].ptr);
					const unsigned char* gPtr = static_cast<const unsigned char*>(prop.fields[i + 1].ptr);
					const unsigned char* bPtr = static_cast<const unsigned char*>(prop.fields[i + 2].ptr);
					const unsigned char* aPtr = static_cast<const unsigned char*>(prop.fields[i + 3].ptr);
					ImVec4 color = ImVec4(*rPtr / 255.0f, *gPtr / 255.0f, *bPtr / 255.0f, *aPtr / 255.0f);
					std::string serializedPropertyData;
					if (ImGui::ColorEdit4(label, (float*)&color, ImGuiColorEditFlags_AlphaBar)) {
					}
					if (ImGui::IsItemActivated()) {
						colorEditOldValue = ImVec4(*rPtr, *gPtr, *bPtr, *aPtr);
					}
					if (ImGui::IsItemEdited()) {
						// Assign value
						unsigned char newR = color.x * 255.0f;
						unsigned char newG = color.y * 255.0f;
						unsigned char newB = color.z * 255.0f;
						unsigned char newA = color.w * 255.0f;
						serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
						engineDLL->setPropertyFieldValue(typeID, prop.index, component, i + 0, (char*)&newR, sizeof(newR));
						engineDLL->setPropertyFieldValue(typeID, prop.index, component, i + 1, (char*)&newG, sizeof(newG));
						engineDLL->setPropertyFieldValue(typeID, prop.index, component, i + 2, (char*)&newB, sizeof(newB));
						engineDLL->setPropertyFieldValue(typeID, prop.index, component, i + 3, (char*)&newA, sizeof(newA));
						engineDLL->onUpdate(component, typeID, prop.index);
					}
					bool colorHasChanged = *rPtr != colorEditOldValue.x || *gPtr != colorEditOldValue.y || *bPtr != colorEditOldValue.z || *aPtr != colorEditOldValue.w;
					if (ImGui::IsItemDeactivatedAfterEdit() && colorHasChanged) {
						undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
						if (!prop.overriden) {
							engineDLL->overrideProperty(entityID, typeID, prop.index);
						}
					}
				}
			}
			else {
				std::size_t fieldCount = prop.fields.size();
				if (fieldCount > 1) {
					ImGui::Text(prop.name.c_str());
					ImGui::Indent(10.0f);
				}
				for (std::size_t i = 0; i < fieldCount; i++) {
					auto& field = prop.fields[i];
					std::string labelString = std::string(field.name) + (prop.overriden ? " (overriden)" : "") + "##" + prop.name + "_" + std::to_string(index) + "_" + std::to_string(i);
					const char* label = labelString.c_str();
					if (field.ptr == nullptr) {
						ImGui::Text(label); // TODO: Display nullptr error
						continue;
					}
					if (field.type == InspectorFieldType::BOOL) {
						bool value = 0;
						memcpy_s(&value, sizeof(value), field.ptr, field.typeSize);
						if (ImGui::Checkbox(label, &value)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)&value, sizeof(value));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}
					}
					else if (field.type == InspectorFieldType::DECIMAL) {
						ImGuiDataType dataType =
						field.typeSize == 4 ? ImGuiDataType_Float :
						ImGuiDataType_Double;
						double value = 0;
						memcpy_s(&value, sizeof(value), field.ptr, field.typeSize);

						if (ImGui::InputScalar(label, dataType, &value, NULL, NULL, "%.1f", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)&value, sizeof(value));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}
					}
					else if (field.type == InspectorFieldType::SIGNED_NUMBER) {
						int64_t value = 0;
						memcpy_s(&value, sizeof(value), field.ptr, field.typeSize);
						ImGuiDataType inputType =
							field.typeSize == 1 ? ImGuiDataType_S8 :
							field.typeSize == 2 ? ImGuiDataType_S16 :
							field.typeSize == 4 ? ImGuiDataType_S32 :
							ImGuiDataType_S64;
						if (ImGui::InputScalar("int", inputType, &value, NULL, NULL, "%d", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)&value, sizeof(value));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}
					}
					else if (field.type == InspectorFieldType::UNSIGNED_NUMBER) {
						uint64_t value = 0;
						memcpy_s(&value, sizeof(value), field.ptr, field.typeSize);
						ImGuiDataType inputType =
							field.typeSize == 1 ? ImGuiDataType_U8 :
							field.typeSize == 2 ? ImGuiDataType_U16 :
							field.typeSize == 4 ? ImGuiDataType_U32 :
							ImGuiDataType_U64;
						if (ImGui::InputScalar(label, inputType, &value, NULL, NULL, "%d", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)&value, sizeof(value));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}
					}
					else if (field.type == InspectorFieldType::STRING) {
						const char* ptr = static_cast<const char*>(field.ptr);
						char buffer[64];
						strncpy_s(buffer, ptr, field.typeSize);

						if (ImGui::InputText(label, buffer, 64, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, buffer, sizeof(buffer));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}
					}
					else if (field.type == InspectorFieldType::WIDE_STRING) {
						const wchar_t* ptr = static_cast<const wchar_t*>(field.ptr);
						char buffer[64];
						wchar_t wbuffer[64];
						std::wstring decodedString(ptr);
						std::string u8 = utf8_encode(decodedString);
						strncpy_s(buffer, u8.c_str(), u8.size());

						if (ImGui::InputText(label, buffer, 64, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							std::size_t outSize;
							std::size_t size = strlen(buffer);
							mbstowcs_s(&outSize, wbuffer, size + 1, buffer, size);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)wbuffer, sizeof(wbuffer));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}
					}
					else if (field.type == InspectorFieldType::IMAGE_PATH) {
						const char* ptr = static_cast<const char*>(field.ptr);
						char buffer[256];
						strncpy_s(buffer, ptr, field.typeSize);

						if (ImGui::InputText(label, buffer, 256, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, buffer, sizeof(buffer));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}

						if (ImGui::BeginDragDropTarget()) {
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIRECTORY_ENTRY")) {
								if (char* data = static_cast<char*>(payload->Data)) { // TODO: Check if data has valid extension
									// Assign value
									std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
									engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, data, sizeof(data));
									engineDLL->onUpdate(component, typeID, prop.index);
									undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
									if (!prop.overriden) {
										engineDLL->overrideProperty(entityID, typeID, prop.index);
									}
								}
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::PushID(typeID);
						if (ImGui::Button("Select image")) {
							std::wstring filePath = getOpenFileName(L"Select A File", L"Image Files\0*.png;*.jpg;*.psd;*.tga;*.bmp\0Any File\0*.*\0", 2);
							if (!filePath.empty()) {
								char sbuffer[256];
								std::string newValue = utf8_encode(filePath);
								strncpy_s(sbuffer, newValue.c_str(), newValue.size());
								// Assign value
								std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
								engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, sbuffer, sizeof(sbuffer));
								engineDLL->onUpdate(component, typeID, prop.index);
								undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
								if (!prop.overriden) {
									engineDLL->overrideProperty(entityID, typeID, prop.index);
								}
							}
						}
						ImGui::PopID();
					}
					else if (field.type == InspectorFieldType::SHADER_PATH) {
						const char* ptr = static_cast<const char*>(field.ptr);
						char buffer[256];
						strncpy_s(buffer, ptr, field.typeSize);

						if (ImGui::InputText(label, buffer, 256, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)buffer, sizeof(buffer));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}
						if (ImGui::BeginDragDropTarget()) {
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIRECTORY_ENTRY")) {
								if (char* data = static_cast<char*>(payload->Data)) { // TODO: Check if data has valid extension
									// Assign value
									std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
									engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)data, sizeof(data));
									engineDLL->onUpdate(component, typeID, prop.index);
									undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
									if (!prop.overriden) {
										engineDLL->overrideProperty(entityID, typeID, prop.index);
									}
								}
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::PushID(typeID);
						if (ImGui::Button("Select shader")) {
							std::wstring filePath = getOpenFileName(L"Select A File", L"Shader Files\0*.vert;*.frag\0Any File\0*.*\0", 2);
							if (!filePath.empty()) {
								char sbuffer[256];
								std::string newValue = utf8_encode(filePath).substr(0, newValue.find_last_of("."));
								strncpy_s(sbuffer, newValue.c_str(), newValue.size());
								// Assign value
								std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
								engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, sbuffer, sizeof(sbuffer));
								engineDLL->onUpdate(component, typeID, prop.index);
								undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
								if (!prop.overriden) {
									engineDLL->overrideProperty(entityID, typeID, prop.index);
								}
							}
						}
						ImGui::PopID();
					}
					else if (field.type == InspectorFieldType::FONT_PATH) {
						const char* fontPath = static_cast<const char*>(field.ptr);

						char buffer[256];
						strncpy_s(buffer, fontPath, field.typeSize);

						if (ImGui::InputText(label, buffer, 256, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
							// Assign value
							std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
							engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)buffer, sizeof(buffer));
							engineDLL->onUpdate(component, typeID, prop.index);
							undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
							if (!prop.overriden) {
								engineDLL->overrideProperty(entityID, typeID, prop.index);
							}
						}
						if (ImGui::BeginDragDropTarget()) {
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIRECTORY_ENTRY")) {
								if (char* data = static_cast<char*>(payload->Data)) { // TODO: Check if data has valid extension
									// Assign value
									std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
									engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, (char*)data, sizeof(data));
									engineDLL->onUpdate(component, typeID, prop.index);
									undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
									if (!prop.overriden) {
										engineDLL->overrideProperty(entityID, typeID, prop.index);
									}
								}
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::PushID(typeID);
						if (ImGui::Button("Select font")) {
							std::wstring filePath = getOpenFileName(L"Select A Font File", L"Font Files\0*.ttf;*.ttc;*.fnt;\0Any File\0*.*\0", 2);
							if (!filePath.empty()) {
								char sbuffer[256];
								std::string newValue = utf8_encode(filePath);
								strncpy_s(sbuffer, newValue.c_str(), newValue.size());
								// Assign value
								std::string serializedPropertyData = engineDLL->writePropertyToBuffer(entityID, typeID, prop.index);
								engineDLL->setPropertyFieldValue(typeID, prop.index, component, i, sbuffer, sizeof(sbuffer));
								engineDLL->onUpdate(component, typeID, prop.index);
								undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(rootEntityID, entityID, typeName, prop.name, std::move(serializedPropertyData), prop.overriden));
								if (!prop.overriden) {
									engineDLL->overrideProperty(entityID, typeID, prop.index);
								}
							}
						}
					}
					else {
						ImGui::Text(label);
					}
				}
				if (fieldCount > 1) {
					ImGui::Unindent(10.0f);
				}
			}
		}
		ImGui::Unindent();
	}
	std::string popupId = "Delete component##" + std::to_string(index);
	if (!closable_group) {
		ImGui::OpenPopup(popupId.c_str());
	}
	
	if (ImGui::BeginPopupModal(popupId.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you want to delete %s?\n\n", typeName.c_str());
		ImGui::Separator();

		if (ImGui::Button("Delete", ImVec2(120, 0))) {
			// Remove component
			ComponentData componentData = engineDLL->getComponent(entityID, typeID);
			std::string serializedComponentData = engineDLL->writeComponentToBuffer(entityID, typeID);
			auto propertyOverrides = engineDLL->getPropertyOverrides(entityID, typeID);
			if (engineDLL->removeComponent(entityID, typeID)) {
				engineDLL->clearPropertyOverrides(entityID, typeID);
				std::string typeName = engineDLL->getTypeName(typeID);
				undoRedoManager->registerUndo(std::make_unique<AddComponentAction>(rootEntityID, entityID, typeName, std::move(serializedComponentData), propertyOverrides));
			}
			else {
				std::cout << "Inspector::renderComponent::ERROR Failed to remove component" << std::endl;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

template<typename T>
bool contains(const std::vector<T>& vec, const T& value) {
	for (const T& v : vec) {
		if (v == value) return true;
	}
	return false;
}

bool filterComponentType(EngineDLL* engineDLL, const std::vector<TypeID>& filter, const TypeID& typeID) {
	std::vector<TypeID> derivedTypeIDs = engineDLL->getDerivedTypeIDs(typeID);
	for (const TypeID& filterID : filter) {
		if (typeID == filterID || contains(derivedTypeIDs, filterID)) {
			return true;
		}
	}
	return false;
}

std::size_t findCaseInsensitive(std::string data, std::string toSearch, std::size_t pos = 0) {
	std::transform(data.begin(), data.end(), data.begin(), std::tolower);
	std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), std::tolower);
	return data.find(toSearch, pos);
}

void renderComponentCombo(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager, EntityTargetData target, char* searchStr) { // TODO: Get list of all types (typeID, name)
	std::vector<TypeID> filter;
	for (ComponentData& componentData : engineDLL->getComponents(target.entityID)) {
		filter.push_back(componentData.typeID);
	}
	ImGui::InputText("##component_search", searchStr, 128);
	ImGui::SetItemDefaultFocus();
	for (ReflectedTypeData& type : engineDLL->getAllReflectedTypes()) {
		std::size_t nameIndex = type.typeName.find_last_of("::");
		std::string shortTypeName = nameIndex == std::string::npos ? type.typeName : type.typeName.substr(nameIndex + 1);
		if (findCaseInsensitive(shortTypeName, searchStr) != 0) continue;
		if (!filterComponentType(engineDLL, filter, type.typeID)) {
			if (ImGui::Selectable(type.typeName.c_str(), false)) {
				// Adding component
				bool overriden = engineDLL->isComponentOverriden(target.entityID, type.typeID);
				if (engineDLL->addComponent(target.entityID, type.typeID)) {
					if (overriden) {
						// TODO: Only load/update component
						EntityID prefabRootEntityID = engineDLL->getNearestPrefabRootEntityID(target.entityID);
						undoRedoManager->setDirty(prefabRootEntityID, true);
						if (!engineDLL->updatePrefab(prefabRootEntityID)) {
							std::cout << "Inspector::renderComponentCombo:ERROR Failed to update prefab " << prefabRootEntityID << " after adding component (" << type.typeName << ") which was previously overriden as removed" << std::endl;
						}
					}
					undoRedoManager->registerUndo(std::make_unique<RemoveComponentAction>(target.rootEntityID, target.entityID, type.typeName));
				}
				else {
					std::cout << "Inspector::renderComponentCombo::ERROR Failed to add component" << std::endl;
				}
			}
		}
	}
}

void Inspector::tick() {
	EntityTargetData target = gameView->getTarget();
	if (target.entityID != prevTarget) { // On target change
		renameActive = false;
	}
	prevTarget = target.entityID;

	ImGui::Begin("Inspector");

	if (engineDLL->isLoaded() && target.entityID != 0) {
		std::string targetName = target.entityName;
		if (renameActive) {
			std::string* value = &renameValue;
			char buffer[64];
			strncpy_s(buffer, value->c_str(), value->size());

			if (ImGui::InputText("##Inspector_Rename_Field", buffer, 64, ImGuiInputTextFlags_EnterReturnsTrue)) {
				renameActive = false;
				if (engineDLL->setEntityName(target.entityID, buffer)) {
					gameView->updateTargetData();
					undoRedoManager->registerUndo(std::make_unique<RenameEntityAction>(target.rootEntityID, target.entityID, buffer));
				}
			}
			ImGui::SetKeyboardFocusHere(0);
			if (!ImGui::IsItemHovered() && ImGui::IsAnyMouseDown()) {
				renameActive = false;
			}
		}
		else {
			ImGui::Text(targetName.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Rename")) {
				renameActive = true;
				renameValue = targetName;
			}
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0, 0, 1));
			if (ImGui::Button("Delete")) {
				popupManager->openDeleteEntity(target.entityID);
			}
			ImGui::PopStyleColor(3);
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_DELETE)) {
			popupManager->openDeleteEntity(target.entityID);
		}
		
		ImGui::BeginChild(targetName.c_str(), ImVec2(0, 0));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		std::size_t i = 0;
		for (ComponentData& component : engineDLL->getComponents(target.entityID)) {
			renderComponent(target.rootEntityID, target.entityID, target.entityName, component.typeID, component.instance, i++);
		}
		static char searchStr[128] = "";
		if (ImGui::Button("Add component", ImVec2(ImGui::GetContentRegionAvailWidth(), 30))) {
			ImGui::OpenPopup("add_component_popup");
			searchStr[0] = '\0';
		}
		if (ImGui::BeginPopup("add_component_popup")) {
			renderComponentCombo(engineDLL, undoRedoManager, target, searchStr);
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
	else {
		ImGui::Text("No target");
	}

	ImGui::End();
}