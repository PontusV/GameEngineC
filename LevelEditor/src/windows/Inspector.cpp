#include "Inspector.h"
#include "GameView.h"
#include "imgui/imgui.h"
#include <Core/Component.h>
#include <Core/Reflection.h>
#include <Core/Scene.h>
#include "utils/file.h"
#include "utils/string.h"
#include <vector>

using namespace Editor;
using namespace Core;

//-------------

template <typename T>
bool vectorIncludes(std::vector<T> vec, T value) {
	for (const T& e : vec) {
		if (e == value) return true;
	}
	return false;
}

//-------------

Inspector::Inspector(GameView* gameView) : gameView(gameView) {}
Inspector::~Inspector() {}

std::vector<Core::ReflectedPropertyData> getProperties(std::size_t typeID, void* instance) {
	std::size_t count = Core::getPropertiesCount(typeID);
	std::vector<Core::ReflectedPropertyData> properties(count);
	Core::getProperties(typeID, instance, &properties[0], count);
	return properties;
}

std::vector<IComponent*> getComponents(IEntityHandle* target) {
	std::size_t count = target->getComponentCount();
	std::vector<IComponent*> components(count);
	target->getIComponents(&components[0], count);
	return components;
}

void renderComponent(IEntityHandle* entity, IComponent* component, std::size_t index) {
	std::size_t typeID = component->getTypeID();
	if (Core::hasAnnotation(typeID, "hideInInspector")) return;
	char typeName[256];
	Core::getTypeName(typeID, typeName, 256);

	bool closable_group = true;
	if (ImGui::CollapsingHeader(typeName, &closable_group, ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();
		for (Core::ReflectedPropertyData& prop : getProperties(typeID, component)) {
			std::string labelString = std::string(prop.name) + "##" + std::to_string(index);
			const char* label = labelString.c_str();

			if (prop.renderer == InspectorFieldRenderType::BOOL) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				ImGui::Checkbox(label, (bool*)ptr);
			}
			else if (prop.renderer == InspectorFieldRenderType::DECIMAL) { // Note: Decimals are always signed
				void* ptr = prop.fieldBuffer[0].dataPtr;
				ImGuiDataType dataType =
					prop.typeName == "float" ? ImGuiDataType_Float :
					prop.typeName == "double" ? ImGuiDataType_Double :
					prop.typeName == "long double" ? ImGuiDataType_Double :
					ImGuiDataType_Float;

				if (ImGui::InputScalar(label, dataType, ptr, NULL, NULL, "%.1f"))
					Core::onUpdate(component, typeID, prop.index);
			}
			else if (prop.renderer == InspectorFieldRenderType::SIGNED_CHAR) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				if (ImGui::InputScalar("char", ImGuiDataType_S8, ptr, NULL, NULL, "%d"))
					Core::onUpdate(component, typeID, prop.index);
			}
			else if (prop.renderer == InspectorFieldRenderType::SIGNED_NUMBER) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				if (ImGui::InputScalar("int", ImGuiDataType_S32, ptr, NULL, NULL, "%d"))
					Core::onUpdate(component, typeID, prop.index);
				//ImGui::InputScalar("input s16", ImGuiDataType_S16, &s16_v, inputs_step ? &s16_one : NULL, NULL, "%d");
				//ImGui::InputScalar("input u16", ImGuiDataType_U16, &u16_v, inputs_step ? &u16_one : NULL, NULL, "%u");
				//ImGui::InputScalar("input s64", ImGuiDataType_S64, &s64_v, inputs_step ? &s64_one : NULL);
				//ImGui::InputScalar("input u64", ImGuiDataType_U64, &u64_v, inputs_step ? &u64_one : NULL);
			}
			else if (prop.renderer == InspectorFieldRenderType::UNSIGNED_CHAR) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				if (ImGui::InputScalar(label, ImGuiDataType_U8, ptr, NULL, NULL, "%d"))
					Core::onUpdate(component, typeID, prop.index);
			}
			else if (prop.renderer == InspectorFieldRenderType::UNSIGNED_NUMBER) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				if (ImGui::InputScalar(label, ImGuiDataType_U32, ptr, NULL, NULL, "%d"))
					Core::onUpdate(component, typeID, prop.index);
			}
			else if (prop.renderer == InspectorFieldRenderType::STRING) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				std::string* value = static_cast<std::string*>(ptr);
				char buffer[64];
				strncpy_s(buffer, value->c_str(), value->size());

				if (ImGui::InputText(label, buffer, 64)) {
					*value = buffer;
					Core::onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::WIDE_STRING) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				std::wstring* value = static_cast<std::wstring*>(ptr);
				char buffer[64];
				std::string u8 = utf8_encode(*value);
				strncpy_s(buffer, u8.c_str(), u8.size());

				if (ImGui::InputText(label, buffer, 64)) {
					*value = utf8_decode(buffer);
					Core::onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::IMAGE_PATH) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				std::string* value = static_cast<std::string*>(ptr);
				char buffer[64];
				strncpy_s(buffer, value->c_str(), value->size());

				if (ImGui::InputText(label, buffer, 64)) {
					*value = buffer;
					Core::onUpdate(component, typeID, prop.index);
				}
				if (ImGui::Button("Select image")) {
					std::wstring filePath = getOpenFileName(L"Select A File", L"Image Files\0*.png;*.jpg;*.psd;*.tga;*.bmp\0Any File\0*.*\0", 2);
					*value = utf8_encode(filePath);
					Core::onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::SHADER_PATH) {
				void* ptr = prop.fieldBuffer[0].dataPtr;
				std::string* value = static_cast<std::string*>(ptr);
				char buffer[64];
				strncpy_s(buffer, value->c_str(), value->size());

				if (ImGui::InputText(label, buffer, 64)) {
					*value = buffer;
					Core::onUpdate(component, typeID, prop.index);
				}
				if (ImGui::Button("Select shader")) {
					std::wstring filePath = getOpenFileName(L"Select A File", L"Shader Files\0*.vert;*.frag\0Any File\0*.*\0", 2);
					if (!filePath.empty()) {
						std::string newValue = utf8_encode(filePath);
						*value = newValue.substr(0, newValue.find_last_of("."));
						Core::onUpdate(component, typeID, prop.index);
					}
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::VECTOR2) {
				float* xPtr = static_cast<float*>(prop.fieldBuffer[0].dataPtr);
				float* yPtr = static_cast<float*>(prop.fieldBuffer[1].dataPtr);
				float vec2f[2] = { *xPtr, *yPtr };
				// TODO: Label x and y
				if (ImGui::InputFloat2(label, vec2f, "%.1f")) {
					*xPtr = vec2f[0];
					*yPtr = vec2f[1];
					Core::onUpdate(component, typeID, prop.index);
				}
			}
			else if (prop.renderer == InspectorFieldRenderType::COLOR) {
				unsigned char* rPtr = static_cast<unsigned char*>(prop.fieldBuffer[0].dataPtr);
				unsigned char* gPtr = static_cast<unsigned char*>(prop.fieldBuffer[1].dataPtr);
				unsigned char* bPtr = static_cast<unsigned char*>(prop.fieldBuffer[2].dataPtr);
				unsigned char* aPtr = static_cast<unsigned char*>(prop.fieldBuffer[3].dataPtr);
				ImVec4 color = ImVec4(*rPtr / 255.0f, *gPtr / 255.0f, *bPtr / 255.0f, *aPtr / 255.0f);
				if (ImGui::ColorEdit4(label, (float*)& color)) {
					*rPtr = color.x * 255.0f;
					*gPtr = color.y * 255.0f;
					*bPtr = color.z * 255.0f;
					*aPtr = color.w * 255.0f;
					Core::onUpdate(component, typeID, prop.index);
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
			entity->removeComponent(typeID);
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

bool filterComponentType(const std::vector<ComponentTypeID>& filter, const ComponentTypeID& typeID) {
	std::size_t count = Core::getDerivedTypeIDsCount(typeID);
	std::vector<ComponentTypeID> derivedTypeIDs(count);
	Core::getDerivedTypeIDs(typeID, &derivedTypeIDs[0], count);
	for (const ComponentTypeID& filterID : filter) {
		if (typeID == filterID || contains(derivedTypeIDs, filterID)) {
			return true;
		}
	}
	return false;
}

void renderComponentCombo(IEntityHandle* target) { // TODO: Get list of all types (typeID, name)
	std::vector<ComponentTypeID> filter;
	for (IComponent* component : getComponents(target)) {
		filter.push_back(component->getTypeID());
	}
	std::size_t count = Core::getAllReflectedTypesCount();
	std::vector<ReflectedTypeData> types(count);
	Core::getAllReflectedTypes(&types[0], count);
	for (ReflectedTypeData& type : types) {
		if (!filterComponentType(filter, type.typeID)) {
			if (ImGui::Selectable(type.typeName, false)) {
				target->addComponent(type.typeID);
			}
		}
	}
}

void Inspector::tick() {
	IEntityHandle* target = gameView->getTarget();
	if (target->getEntity() != prevTarget) { // On target change
		renameActive = false;
	}
	prevTarget = target->getEntity();

	ImGui::Begin("Inspector");

	if (target->refresh()) {
		if (renameActive) {
			std::string* value = &renameValue;
			char buffer[64];
			strncpy_s(buffer, value->c_str(), value->size());

			if (ImGui::InputText("##Inspector_Rename_Field", buffer, 64, ImGuiInputTextFlags_EnterReturnsTrue)) {
				renameActive = false;
				target->renameEntity(buffer);
			}
			ImGui::SetKeyboardFocusHere(0);
			if (!ImGui::IsItemHovered() && ImGui::IsAnyMouseDown()) {
				renameActive = false;
			}
		}
		else {
			ImGui::Text(target->getEntityName());
			ImGui::SameLine();
			if (ImGui::Button("Rename")) {
				renameActive = true;
				renameValue = target->getEntityName();
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
			ImGui::Text("Are you sure you want to delete %s?\n\n", target->getEntityName());
			ImGui::Separator();

			if (ImGui::Button("Delete", ImVec2(120, 0))) {
				if (IScene* scene = target->getIScene()) {
					scene->destroyEntity(target->getEntity());
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
		ImGui::BeginChild(target->getEntityName(), ImVec2(0, 0));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		std::size_t i = 0;
		for (IComponent* component : getComponents(target)) {
			renderComponent(target, component, i++);
		}
		if (ImGui::Button("Add component", ImVec2(ImGui::GetContentRegionAvailWidth(), 30)))
			ImGui::OpenPopup("add_component_popup");
		if (ImGui::BeginPopup("add_component_popup")) {
			renderComponentCombo(target); // TODO: Get list of all types (typeID, name)
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