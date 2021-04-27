#include "Inspector.h"
#include "GameView.h"
#include "UndoRedo.h"
#include "EngineDLL.h"
#include "imgui/imgui.h"
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

Inspector::Inspector(EngineDLL* engineDLL, GameView* gameView, UndoRedoManager* undoRedoManager) : engineDLL(engineDLL), gameView(gameView), undoRedoManager(undoRedoManager) {}
Inspector::~Inspector() {}

template<typename T>
void safeAssignPropValue(ReflectedFieldData& field, T&& value) {
	std::memset(field.ptr, 0, field.size);
	std::memcpy(field.ptr, &value, std::min(field.size, sizeof(value)));
}

void Inspector::renderComponent(EntityID entityID, std::string entityName, std::size_t sceneIndex, std::size_t typeID, void* component, std::size_t index) {
	if (engineDLL->hasAnnotation(typeID, "hideInInspector")) return;
	std::string typeName = engineDLL->getTypeName(typeID);

	bool closable_group = true;
	if (ImGui::CollapsingHeader(typeName.c_str(), &closable_group, ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();
		for (ReflectedPropertyData& prop : engineDLL->getProperties(typeID, component)) {
			std::string labelString = std::string(prop.name) + "##" + std::to_string(index);
			const char* label = labelString.c_str();

			if (prop.renderer == InspectorFieldRenderType::BOOL) {
				bool* ptr = static_cast<bool*>(prop.fields[0].ptr);
				bool value = *ptr;
				if (ImGui::Checkbox(label, &value)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					safeAssignPropValue(prop.fields[0], value);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::DECIMAL) { // Note: Decimals are always signed
				ImGuiDataType dataType =
					prop.typeName == "float" ? ImGuiDataType_Float :
					prop.typeName == "double" ? ImGuiDataType_Double :
					prop.typeName == "long double" ? ImGuiDataType_Double :
					ImGuiDataType_Float;
				if (dataType == ImGuiDataType_Float) {
					float* ptr = static_cast<float*>(prop.fields[0].ptr);
					float value = *ptr;

					if (ImGui::InputScalar(label, dataType, &value, NULL, NULL, "%.1f", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue )) {
						// Assign value
						auto prevPropData = createPropData(prop);
						undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
						safeAssignPropValue(prop.fields[0], value);
						engineDLL->onUpdate(component, typeID, prop.index);
					}
				}
				else {
					double* ptr = static_cast<double*>(prop.fields[0].ptr);
					double value = *ptr;

					if (ImGui::InputScalar(label, dataType, ptr, NULL, NULL, "%.1f", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
						// Assign value
						auto prevPropData = createPropData(prop);
						undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
						safeAssignPropValue(prop.fields[0], value);
						engineDLL->onUpdate(component, typeID, prop.index);
					}
					//ImGui::IsItemDeactivatedAfterEdit()
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::SIGNED_CHAR) {
				int8_t* ptr = static_cast<int8_t*>(prop.fields[0].ptr);
				int8_t value = *ptr;
				if (ImGui::InputScalar("char", ImGuiDataType_S8, &value, NULL, NULL, "%d", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					safeAssignPropValue(prop.fields[0], value);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::SIGNED_NUMBER) {
				int32_t* ptr = static_cast<int32_t*>(prop.fields[0].ptr);
				int32_t value = *ptr;
				if (ImGui::InputScalar("int", ImGuiDataType_S32, &value, NULL, NULL, "%d", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					safeAssignPropValue(prop.fields[0], value);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				//ImGui::InputScalar("input s16", ImGuiDataType_S16, &s16_v, inputs_step ? &s16_one : NULL, NULL, "%d");
				//ImGui::InputScalar("input u16", ImGuiDataType_U16, &u16_v, inputs_step ? &u16_one : NULL, NULL, "%u");
				//ImGui::InputScalar("input s64", ImGuiDataType_S64, &s64_v, inputs_step ? &s64_one : NULL);
				//ImGui::InputScalar("input u64", ImGuiDataType_U64, &u64_v, inputs_step ? &u64_one : NULL);
			}
			else if (prop.renderer == InspectorFieldRenderType::UNSIGNED_CHAR) {
				uint8_t* ptr = static_cast<uint8_t*>(prop.fields[0].ptr);
				uint8_t value = *ptr;
				if (ImGui::InputScalar(label, ImGuiDataType_U8, &value, NULL, NULL, "%d", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					safeAssignPropValue(prop.fields[0], value);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::UNSIGNED_NUMBER) {
				uint32_t* ptr = static_cast<uint32_t*>(prop.fields[0].ptr);
				uint32_t value = *ptr;
				if (ImGui::InputScalar(label, ImGuiDataType_U32, &value, NULL, NULL, "%d", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					safeAssignPropValue(prop.fields[0], value);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::STRING) {
				assert(sizeof(std::string) == prop.fields[0].size);
				std::string* ptr = static_cast<std::string*>(prop.fields[0].ptr);
				char buffer[64];
				strncpy_s(buffer, ptr->c_str(), ptr->size());

				if (ImGui::InputText(label, buffer, 64, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					*ptr = buffer;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::WIDE_STRING) {
				assert(sizeof(std::wstring) == prop.fields[0].size);
				std::wstring* ptr = static_cast<std::wstring*>(prop.fields[0].ptr);
				char buffer[64];
				std::string u8 = utf8_encode(*ptr);
				strncpy_s(buffer, u8.c_str(), u8.size());

				if (ImGui::InputText(label, buffer, 64, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					std::wstring value = utf8_decode(buffer);
					*ptr = value;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::IMAGE_PATH) {
				assert(sizeof(std::string) == prop.fields[0].size);
				std::string* ptr = static_cast<std::string*>(prop.fields[0].ptr);
				char buffer[256];
				strncpy_s(buffer, ptr->c_str(), ptr->size());

				if (ImGui::InputText(label, buffer, 256, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					*ptr = buffer;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				ImGui::PushID(typeID);
				if (ImGui::Button("Select image")) {
					std::wstring filePath = getOpenFileName(L"Select A File", L"Image Files\0*.png;*.jpg;*.psd;*.tga;*.bmp\0Any File\0*.*\0", 2);
					if (!filePath.empty()) {
						// Assign value
						auto prevPropData = createPropData(prop);
						undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
						*ptr = utf8_encode(filePath);
						engineDLL->onUpdate(component, typeID, prop.index);
					}
				}
				ImGui::PopID();
			}
			else if (prop.renderer == InspectorFieldRenderType::SHADER_PATH) {
				assert(sizeof(std::string) == prop.fields[0].size);
				std::string* ptr = static_cast<std::string*>(prop.fields[0].ptr);
				char buffer[256];
				strncpy_s(buffer, ptr->c_str(), ptr->size());

				if (ImGui::InputText(label, buffer, 256, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					*ptr = buffer;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				ImGui::PushID(typeID);
				if (ImGui::Button("Select shader")) {
					std::wstring filePath = getOpenFileName(L"Select A File", L"Shader Files\0*.vert;*.frag\0Any File\0*.*\0", 2);
					if (!filePath.empty()) {
						// Assign value
						auto prevPropData = createPropData(prop);
						undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
						std::string newValue = utf8_encode(filePath);
						*ptr = newValue.substr(0, newValue.find_last_of("."));
						engineDLL->onUpdate(component, typeID, prop.index);
					}
				}
				ImGui::PopID();
			}
			else if (prop.renderer == InspectorFieldRenderType::VECTOR2) {
				float* xPtr = static_cast<float*>(prop.fields[0].ptr);
				float* yPtr = static_cast<float*>(prop.fields[1].ptr);
				float vec2f[2] = { *xPtr, *yPtr };
				// TODO: Label x and y
				if (ImGui::InputFloat2(label, vec2f, "%.1f", ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					safeAssignPropValue(prop.fields[0], vec2f[0]);
					safeAssignPropValue(prop.fields[1], vec2f[1]);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::COLOR) {
				unsigned char* rPtr = static_cast<unsigned char*>(prop.fields[0].ptr);
				unsigned char* gPtr = static_cast<unsigned char*>(prop.fields[1].ptr);
				unsigned char* bPtr = static_cast<unsigned char*>(prop.fields[2].ptr);
				unsigned char* aPtr = static_cast<unsigned char*>(prop.fields[3].ptr);
				ImVec4 color = ImVec4(*rPtr / 255.0f, *gPtr / 255.0f, *bPtr / 255.0f, *aPtr / 255.0f);
				if (ImGui::ColorEdit4(label, (float*)&color, ImGuiColorEditFlags_AlphaBar)) {
				}
				if (ImGui::IsItemActivated()) {
					colorEditOldValue = ImVec4(*rPtr, *gPtr, *bPtr, *aPtr);
				}
				if (ImGui::IsItemEdited()) {
					// Assign value
					safeAssignPropValue<unsigned char>(prop.fields[0], color.x * 255.0f);
					safeAssignPropValue<unsigned char>(prop.fields[1], color.y * 255.0f);
					safeAssignPropValue<unsigned char>(prop.fields[2], color.z * 255.0f);
					safeAssignPropValue<unsigned char>(prop.fields[3], color.w * 255.0f);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				bool colorHasChanged = *rPtr != colorEditOldValue.x || *gPtr != colorEditOldValue.y || *bPtr != colorEditOldValue.z || *aPtr != colorEditOldValue.w;
				if (ImGui::IsItemDeactivatedAfterEdit() && colorHasChanged) {
					auto prevPropData = createPropDataFromValues((unsigned char)colorEditOldValue.x, (unsigned char)colorEditOldValue.y, (unsigned char)colorEditOldValue.z, (unsigned char)colorEditOldValue.w);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::FONT) {
				assert(sizeof(std::string) == prop.fields[0].size);
				std::string* fontPath = static_cast<std::string*>(prop.fields[0].ptr);
				int* fontSize = static_cast<int*>(prop.fields[1].ptr);

				char buffer[256];
				strncpy_s(buffer, fontPath->c_str(), fontPath->size());

				std::string pathLabel = labelString;
				pathLabel.insert(labelString.find_first_of("#"), "_path");
				std::string sizeLabel = labelString;
				sizeLabel.insert(labelString.find_first_of("#"), "_size");
				if (ImGui::InputText(pathLabel.append("_path").c_str(), buffer, 256, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					*fontPath = buffer;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				if (ImGui::Button("Select font")) {
					std::wstring filePath = getOpenFileName(L"Select A Font File", L"Font Files\0*.ttf;*.ttc;*.fnt;\0Any File\0*.*\0", 2);
					if (!filePath.empty()) {
						// Assign value
						auto prevPropData = createPropData(prop);
						undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
						std::string newValue = utf8_encode(filePath);
						*fontPath = newValue;
						engineDLL->onUpdate(component, typeID, prop.index);
					}
				}
				int fontSizeValue = *fontSize;
				if (ImGui::InputInt(sizeLabel.append("_size").c_str(), &fontSizeValue, 1, ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_EnterReturnsTrue)) {
					// Assign value
					auto prevPropData = createPropData(prop);
					undoRedoManager->registerUndo(std::make_unique<PropertyAssignAction>(sceneIndex, entityName, typeName, prop.name, std::move(prevPropData)));
					safeAssignPropValue(prop.fields[1], fontSizeValue);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else {
				ImGui::Text(label);
			}
		}
		ImGui::Unindent();
	}
	std::string popupId = "Delete component##" + std::to_string(index);
	if (!closable_group) {
		ImGui::OpenPopup(popupId.c_str());
	}
	
	if (ImGui::BeginPopupModal(popupId.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you want to delete %s?\n\n", typeName);
		ImGui::Separator();

		if (ImGui::Button("Delete", ImVec2(120, 0))) {
			if (engineDLL->removeComponent(entityID, sceneIndex, typeID)) {
				std::string entityName = engineDLL->getEntityName(entityID);
				std::string typeName = engineDLL->getTypeName(typeID);
				undoRedoManager->registerUndo(std::make_unique<AddComponentAction>(sceneIndex, entityName, typeName));
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

void renderComponentCombo(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager, EntityTargetData target) { // TODO: Get list of all types (typeID, name)
	std::vector<TypeID> filter;
	for (ComponentData& componentData : engineDLL->getComponents(target.entityID)) {
		filter.push_back(componentData.typeID);
	}
	for (ReflectedTypeData& type : engineDLL->getAllReflectedTypes()) {
		if (!filterComponentType(engineDLL, filter, type.typeID)) {
			if (ImGui::Selectable(type.typeName.c_str(), false)) {
				if (engineDLL->addComponent(target.entityID, target.sceneIndex, type.typeID)) {
					undoRedoManager->registerUndo(std::make_unique<RemoveComponentAction>(target.sceneIndex, target.entityName, type.typeName));
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
				if (engineDLL->renameEntity(target.entityID, buffer)) {
					gameView->updateTargetData();
					undoRedoManager->registerUndo(std::make_unique<RenameEntityAction>(target.sceneIndex, target.entityName, buffer));
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
				ImGui::OpenPopup("Delete_target_entity");
			}
			ImGui::PopStyleColor(3);
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_DELETE)) { // TODO: Move this and popup somewhere else?
			ImGui::OpenPopup("Delete_target_entity");
		}
		if (ImGui::BeginPopupModal("Delete_target_entity", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Are you sure you want to delete %s?\n\n", targetName);
			ImGui::Separator();

			if (ImGui::Button("Delete", ImVec2(120, 0))) {
				if (target.entityID != 0) {
					auto blueprint = EntityBlueprint::createFromEntity(engineDLL, target.entityID);
					if (engineDLL->destroyEntity(target.sceneIndex, target.entityID)) {
						undoRedoManager->registerUndo(std::make_unique<CreateEntityAction>(target.sceneIndex, target.entityName, std::move(blueprint)));
						gameView->releaseTarget();
					}
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
		ImGui::BeginChild(targetName.c_str(), ImVec2(0, 0));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		std::size_t i = 0;
		for (ComponentData& component : engineDLL->getComponents(target.entityID)) {
			renderComponent(target.entityID, target.entityName, target.sceneIndex, component.typeID, component.instance, i++);
		}
		if (ImGui::Button("Add component", ImVec2(ImGui::GetContentRegionAvailWidth(), 30)))
			ImGui::OpenPopup("add_component_popup");
		if (ImGui::BeginPopup("add_component_popup")) {
			renderComponentCombo(engineDLL, undoRedoManager, target);
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