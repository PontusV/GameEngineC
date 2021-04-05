#include "Inspector.h"
#include "GameView.h"
#include "EngineDLL.h"
#include "imgui/imgui.h"
#include "utils/file.h"
#include "utils/string.h"
#include <vector>

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

Inspector::Inspector(EngineDLL* engineDLL, GameView* gameView) : engineDLL(engineDLL), gameView(gameView) {}
Inspector::~Inspector() {}

void renderComponent(EngineDLL* engineDLL, EntityID entityID, std::size_t sceneIndex, std::size_t typeID, void* component, std::size_t index) {
	if (engineDLL->hasAnnotation(typeID, "hideInInspector")) return;
	std::string typeName = engineDLL->getTypeName(typeID);

	bool closable_group = true;
	if (ImGui::CollapsingHeader(typeName.c_str(), &closable_group, ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();
		for (ReflectedPropertyData& prop : engineDLL->getProperties(typeID, component)) {
			std::string labelString = std::string(prop.name) + "##" + std::to_string(index);
			const char* label = labelString.c_str();

			if (prop.renderer == InspectorFieldRenderType::BOOL) {
				void* ptr = prop.fieldPtrs[0];
				ImGui::Checkbox(label, (bool*)ptr);
			}
			else if (prop.renderer == InspectorFieldRenderType::DECIMAL) { // Note: Decimals are always signed
				void* ptr = prop.fieldPtrs[0];
				ImGuiDataType dataType =
					prop.typeName == "float" ? ImGuiDataType_Float :
					prop.typeName == "double" ? ImGuiDataType_Double :
					prop.typeName == "long double" ? ImGuiDataType_Double :
					ImGuiDataType_Float;

				if (ImGui::InputScalar(label, dataType, ptr, NULL, NULL, "%.1f"))
					engineDLL->onUpdate(component, typeID, prop.index);
			}
			else if (prop.renderer == InspectorFieldRenderType::SIGNED_CHAR) {
				void* ptr = prop.fieldPtrs[0];
				if (ImGui::InputScalar("char", ImGuiDataType_S8, ptr, NULL, NULL, "%d"))
					engineDLL->onUpdate(component, typeID, prop.index);
			}
			else if (prop.renderer == InspectorFieldRenderType::SIGNED_NUMBER) {
				void* ptr = prop.fieldPtrs[0];
				if (ImGui::InputScalar("int", ImGuiDataType_S32, ptr, NULL, NULL, "%d"))
					engineDLL->onUpdate(component, typeID, prop.index);
				//ImGui::InputScalar("input s16", ImGuiDataType_S16, &s16_v, inputs_step ? &s16_one : NULL, NULL, "%d");
				//ImGui::InputScalar("input u16", ImGuiDataType_U16, &u16_v, inputs_step ? &u16_one : NULL, NULL, "%u");
				//ImGui::InputScalar("input s64", ImGuiDataType_S64, &s64_v, inputs_step ? &s64_one : NULL);
				//ImGui::InputScalar("input u64", ImGuiDataType_U64, &u64_v, inputs_step ? &u64_one : NULL);
			}
			else if (prop.renderer == InspectorFieldRenderType::UNSIGNED_CHAR) {
				void* ptr = prop.fieldPtrs[0];
				if (ImGui::InputScalar(label, ImGuiDataType_U8, ptr, NULL, NULL, "%d"))
					engineDLL->onUpdate(component, typeID, prop.index);
			}
			else if (prop.renderer == InspectorFieldRenderType::UNSIGNED_NUMBER) {
				void* ptr = prop.fieldPtrs[0];
				if (ImGui::InputScalar(label, ImGuiDataType_U32, ptr, NULL, NULL, "%d"))
					engineDLL->onUpdate(component, typeID, prop.index);
			}
			else if (prop.renderer == InspectorFieldRenderType::STRING) {
				void* ptr = prop.fieldPtrs[0];
				std::string* value = static_cast<std::string*>(ptr);
				char buffer[64];
				strncpy_s(buffer, value->c_str(), value->size());

				if (ImGui::InputText(label, buffer, 64)) {
					*value = buffer;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::WIDE_STRING) {
				void* ptr = prop.fieldPtrs[0];
				std::wstring* value = static_cast<std::wstring*>(ptr);
				char buffer[64];
				std::string u8 = utf8_encode(*value);
				strncpy_s(buffer, u8.c_str(), u8.size());

				if (ImGui::InputText(label, buffer, 64)) {
					*value = utf8_decode(buffer);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::IMAGE_PATH) {
				void* ptr = prop.fieldPtrs[0];
				std::string* value = static_cast<std::string*>(ptr);
				char buffer[256];
				strncpy_s(buffer, value->c_str(), value->size());

				if (ImGui::InputText(label, buffer, 256)) {
					*value = buffer;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				ImGui::PushID(typeID);
				if (ImGui::Button("Select image")) {
					std::wstring filePath = getOpenFileName(L"Select A File", L"Image Files\0*.png;*.jpg;*.psd;*.tga;*.bmp\0Any File\0*.*\0", 2);
					*value = utf8_encode(filePath);
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				ImGui::PopID();
			}
			else if (prop.renderer == InspectorFieldRenderType::SHADER_PATH) {
				void* ptr = prop.fieldPtrs[0];
				std::string* value = static_cast<std::string*>(ptr);
				char buffer[256];
				strncpy_s(buffer, value->c_str(), value->size());

				if (ImGui::InputText(label, buffer, 256)) {
					*value = buffer;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				ImGui::PushID(typeID);
				if (ImGui::Button("Select shader")) {
					std::wstring filePath = getOpenFileName(L"Select A File", L"Shader Files\0*.vert;*.frag\0Any File\0*.*\0", 2);
					if (!filePath.empty()) {
						std::string newValue = utf8_encode(filePath);
						*value = newValue.substr(0, newValue.find_last_of("."));
						engineDLL->onUpdate(component, typeID, prop.index);
					}
				}
				ImGui::PopID();
			}
			else if (prop.renderer == InspectorFieldRenderType::VECTOR2) {
				float* xPtr = static_cast<float*>(prop.fieldPtrs[0]);
				float* yPtr = static_cast<float*>(prop.fieldPtrs[1]);
				float vec2f[2] = { *xPtr, *yPtr };
				// TODO: Label x and y
				if (ImGui::InputFloat2(label, vec2f, "%.1f")) {
					*xPtr = vec2f[0];
					*yPtr = vec2f[1];
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::COLOR) {
				unsigned char* rPtr = static_cast<unsigned char*>(prop.fieldPtrs[0]);
				unsigned char* gPtr = static_cast<unsigned char*>(prop.fieldPtrs[1]);
				unsigned char* bPtr = static_cast<unsigned char*>(prop.fieldPtrs[2]);
				unsigned char* aPtr = static_cast<unsigned char*>(prop.fieldPtrs[3]);
				ImVec4 color = ImVec4(*rPtr / 255.0f, *gPtr / 255.0f, *bPtr / 255.0f, *aPtr / 255.0f);
				if (ImGui::ColorEdit4(label, (float*)& color)) {
					*rPtr = color.x * 255.0f;
					*gPtr = color.y * 255.0f;
					*bPtr = color.z * 255.0f;
					*aPtr = color.w * 255.0f;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::FONT) {
				std::string* fontPath = static_cast<std::string*>(prop.fieldPtrs[0]);
				int* fontSize = static_cast<int*>(prop.fieldPtrs[1]);

				char buffer[256];
				strncpy_s(buffer, fontPath->c_str(), fontPath->size());

				std::string pathLabel = labelString;
				pathLabel.insert(labelString.find_first_of("#"), "_path");
				std::string sizeLabel = labelString;
				sizeLabel.insert(labelString.find_first_of("#"), "_size");
				if (ImGui::InputText(pathLabel.append("_path").c_str(), buffer, 256)) {
					*fontPath = buffer;
					engineDLL->onUpdate(component, typeID, prop.index);
				}
				if (ImGui::Button("Select font")) {
					std::wstring filePath = getOpenFileName(L"Select A Font File", L"Font Files\0*.ttf;*.ttc;*.fnt;\0Any File\0*.*\0", 2);
					if (!filePath.empty()) {
						std::string newValue = utf8_encode(filePath);
						*fontPath = newValue;
						engineDLL->onUpdate(component, typeID, prop.index);
					}
				}
				int sizeBuffer[1] = { *fontSize };
				if (ImGui::InputInt(sizeLabel.append("_size").c_str(), sizeBuffer, 1)) {
					*fontSize = sizeBuffer[0];
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
			engineDLL->removeComponent(entityID, sceneIndex, typeID);
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

void renderComponentCombo(EngineDLL* engineDLL, EntityTargetData target) { // TODO: Get list of all types (typeID, name)
	std::vector<TypeID> filter;
	for (ComponentData& componentData : engineDLL->getComponents(target.entityID)) {
		filter.push_back(componentData.typeID);
	}
	for (ReflectedTypeData& type : engineDLL->getAllReflectedTypes()) {
		if (!filterComponentType(engineDLL, filter, type.typeID)) {
			if (ImGui::Selectable(type.typeName.c_str(), false)) {
				engineDLL->addComponent(target.entityID, target.sceneIndex, type.typeID);
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
				engineDLL->renameEntity(target.entityID, buffer);
				target.entityName = buffer;
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
		if (ImGui::BeginPopupModal("Delete_target_entity", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Are you sure you want to delete %s?\n\n", targetName);
			ImGui::Separator();

			if (ImGui::Button("Delete", ImVec2(120, 0))) {
				if (target.entityID != 0) {
					engineDLL->destroyEntity(target.sceneIndex, target.entityID);
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
			renderComponent(engineDLL, target.entityID, target.sceneIndex, component.typeID, component.instance, i++);
		}
		if (ImGui::Button("Add component", ImVec2(ImGui::GetContentRegionAvailWidth(), 30)))
			ImGui::OpenPopup("add_component_popup");
		if (ImGui::BeginPopup("add_component_popup")) {
			renderComponentCombo(engineDLL, target);
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