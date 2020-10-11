#include "Inspector.h"
#include "GameView.h"
#include "imgui/imgui.h"
#include "ReflectionPolymorph.generated.h"
#include "utils/file.h"
#include "utils/string.h"

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

void renderComponent(Component* component, std::size_t index) {
	Mirror::Class type = component->getType();
	if (type.hasAnnotation("hideInInspector")) return;

	bool closable_group = true;
	if (ImGui::CollapsingHeader(type.name.c_str(), &closable_group, ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();
		for (Mirror::Property& prop : type.properties) {

			void* ptr = Mirror::getPointer(prop, component, type.typeID);
			std::string labelString = prop.name + "##" + std::to_string(index);
			const char* label = labelString.c_str();

			if (prop.type.isBool()) {
				ImGui::Checkbox(label, (bool*)ptr);
			}
			else if (prop.type.isNumber()) {
				if (prop.type.isSignedNumber()) {
					// Signed
					if (prop.type.isChar()) {
						if (ImGui::InputScalar("char", ImGuiDataType_S8, ptr, NULL, NULL, "%d"))
							Mirror::onUpdate(component, type.typeID, prop);
					}
					else if (prop.type.isDecimal()) {
						ImGuiDataType dataType =
							prop.type.name == "float" ? ImGuiDataType_Float :
							prop.type.name == "double" ? ImGuiDataType_Double :
							prop.type.name == "long double" ? ImGuiDataType_Double :
							ImGuiDataType_Float;

						if (ImGui::InputScalar(label, dataType, ptr, NULL, NULL, "%.1f"))
							Mirror::onUpdate(component, type.typeID, prop);
					}
					else {
						if (ImGui::InputScalar("int", ImGuiDataType_S32, ptr, NULL, NULL, "%d"))
							Mirror::onUpdate(component, type.typeID, prop);
						//ImGui::InputScalar("input s16", ImGuiDataType_S16, &s16_v, inputs_step ? &s16_one : NULL, NULL, "%d");
						//ImGui::InputScalar("input u16", ImGuiDataType_U16, &u16_v, inputs_step ? &u16_one : NULL, NULL, "%u");
						//ImGui::InputScalar("input s64", ImGuiDataType_S64, &s64_v, inputs_step ? &s64_one : NULL);
						//ImGui::InputScalar("input u64", ImGuiDataType_U64, &u64_v, inputs_step ? &u64_one : NULL);
					}
				}
				else {
					// Unsigned
					if (prop.type.isChar()) {
						if (ImGui::InputScalar(label, ImGuiDataType_U8, ptr, NULL, NULL, "%d"))
							Mirror::onUpdate(component, type.typeID, prop);
					}
					else {
						if (ImGui::InputScalar(label, ImGuiDataType_U32, ptr, NULL, NULL, "%d"))
							Mirror::onUpdate(component, type.typeID, prop);
					}
				}
			}
			else if (prop.type.isString()) {
				if (vectorIncludes<std::string>(prop.getAnnotationValue("Category"), "ImagePath")) {
					std::string* value = static_cast<std::string*>(ptr);
					char buffer[64];
					strncpy_s(buffer, value->c_str(), value->size());

					if (ImGui::InputText(label, buffer, 64)) {
						*value = buffer;
						Mirror::onUpdate(component, type.typeID, prop);
					}
					if (ImGui::Button("Select image")) {
						std::wstring filePath = getOpenFileName(L"Select A File", L"Image Files\0*.png;*.jpg;*.psd;*.tga;*.bmp\0Any File\0*.*\0", 2);
						*value = utf8_encode(filePath);
						Mirror::onUpdate(component, type.typeID, prop);
					}
				}
				else if (vectorIncludes<std::string>(prop.getAnnotationValue("Category"), "ShaderPath")) {
					std::string* value = static_cast<std::string*>(ptr);
					char buffer[64];
					strncpy_s(buffer, value->c_str(), value->size());

					if (ImGui::InputText(label, buffer, 64)) {
						*value = buffer;
						Mirror::onUpdate(component, type.typeID, prop);
					}
					if (ImGui::Button("Select shader")) {
						std::wstring filePath = getOpenFileName(L"Select A File", L"Shader Files\0*.vert;*.frag\0Any File\0*.*\0", 2);
						if (!filePath.empty()) {
							std::string newValue = utf8_encode(filePath);
							*value = newValue.substr(0, newValue.find_last_of("."));
							Mirror::onUpdate(component, type.typeID, prop);
						}
					}
				}
				else {
					std::string* value = static_cast<std::string*>(ptr);
					char buffer[64];
					strncpy_s(buffer, value->c_str(), value->size());

					if (ImGui::InputText(label, buffer, 64)) {
						*value = buffer;
						Mirror::onUpdate(component, type.typeID, prop);
					}
				}
			}
			else if (prop.type.isWideString()) {
				std::wstring* value = static_cast<std::wstring*>(ptr);
				char buffer[64];
				std::string u8 = utf8_encode(*value);
				strncpy_s(buffer, u8.c_str(), u8.size());

				if (ImGui::InputText(label, buffer, 64)) {
					*value = utf8_decode(buffer);
					Mirror::onUpdate(component, type.typeID, prop);
				}
			}
			else if (prop.type.name == "Core::Vector2") {
				Core::Vector2* value = static_cast<Core::Vector2*>(ptr);
				float vec2f[2] = { value->x, value->y };
				// TODO: Label x and y
				if (ImGui::InputFloat2(label, vec2f, "%.1f")) {
					value->x = vec2f[0];
					value->y = vec2f[1];
					Mirror::onUpdate(component, type.typeID, prop);
				}
			}
			else if (prop.type.name == "Core::Color") {
				Core::Color* value = static_cast<Core::Color*>(ptr);
				ImVec4 color = ImVec4(value->r / 255.0f, value->g / 255.0f, value->b / 255.0f, value->a / 255.0f);
				if (ImGui::ColorEdit4(label, (float*)& color)) {
					value->r = color.x * 255.0f;
					value->g = color.y * 255.0f;
					value->b = color.z * 255.0f;
					value->a = color.w * 255.0f;
					Mirror::onUpdate(component, type.typeID, prop);
				}
			}
			else if (prop.type.name == "Core::Shader") {
				/*std::string* value = static_cast<std::string*>(ptr);
				char buffer[64];
				strncpy_s(buffer, value->c_str(), value->size());

				if (ImGui::InputText(label, buffer, 64)) {
					*value = buffer;
					Mirror::onUpdate(component, type.typeID, prop);
				}
				if (ImGui::Button("Select shader")) {
					std::wstring filePath = getOpenFileName(L"Select A File", L"Image Files\0*.png;*.jpg;*.psd;*.tga;*.bmp\0Any File\0*.*\0", 2);
					*value = utf8_encode(filePath);
					Mirror::onUpdate(component, type.typeID, prop);
				}*/
				ImGui::Text(label);
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
		ImGui::Text("Are you sure you want to delete %s?\n\n", component->getType().name.c_str());
		ImGui::Separator();

		if (ImGui::Button("Delete", ImVec2(120, 0))) {
			EntityHandle target = component->getOwner();
			target.removeComponent(component->getType().typeID);
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
	for (const ComponentTypeID& filterID : filter) {
		if (typeID == filterID || contains(Mirror::polyGetDerivedTypeIDs(typeID), filterID)) {
			return true;
		}
	}
	return false;
}

template<typename... Ts>
void renderComponentCombo(EntityHandle target, std::vector<ComponentTypeID> filter, Mirror::TypeList<Ts...>) {} // End
template<typename T, typename... Ts>
typename std::enable_if_t<std::is_base_of<Component, T>::value&& std::is_default_constructible<T>::value && !std::is_abstract<T>::value && !std::is_same<T, ChildManager>::value && !std::is_same<T, ParentEntity>::value, void> renderComponentCombo(EntityHandle target, std::vector<ComponentTypeID> filter, Mirror::TypeList<T, Ts...>);
template<typename T, typename... Ts>
typename std::enable_if_t<!std::is_base_of<Component, T>::value || !std::is_default_constructible<T>::value || std::is_abstract<T>::value || std::is_same<T, ChildManager>::value || std::is_same<T, ParentEntity>::value, void> renderComponentCombo(EntityHandle target, std::vector<ComponentTypeID> filter, Mirror::TypeList<T, Ts...>);

template<typename T, typename... Ts>
typename std::enable_if_t<std::is_base_of<Component, T>::value&& std::is_default_constructible<T>::value && !std::is_abstract<T>::value && !std::is_same<T, ChildManager>::value && !std::is_same<T, ParentEntity>::value, void> renderComponentCombo(EntityHandle target, std::vector<ComponentTypeID> filter, Mirror::TypeList<T, Ts...>) {
	Mirror::Class type = T::getClassType();
	if (!filterComponentType(filter, type.typeID)) {
		if (ImGui::Selectable(type.name.c_str(), false)) {
			target.addComponent<T>();
		}
	}
	renderComponentCombo(target, filter, Mirror::TypeList<Ts...>{}); // Continue
}

template<typename T, typename... Ts>
typename std::enable_if_t<!std::is_base_of<Component, T>::value || !std::is_default_constructible<T>::value || std::is_abstract<T>::value || std::is_same<T, ChildManager>::value || std::is_same<T, ParentEntity>::value, void> renderComponentCombo(EntityHandle target, std::vector<ComponentTypeID> filter, Mirror::TypeList<T, Ts...>) {
	renderComponentCombo(target, filter, Mirror::TypeList<Ts...>{}); // Continue
}

void Inspector::tick() {
	EntityHandle& target = gameView->getTarget();
	if (target.getEntity() != prevTarget) { // On target change
		renameActive = false;
	}
	prevTarget = target.getEntity();

	ImGui::Begin("Inspector");

	if (target.refresh()) {
		if (renameActive) {
			std::string* value = &renameValue;
			char buffer[64];
			strncpy_s(buffer, value->c_str(), value->size());

			if (ImGui::InputText("##Inspector_Rename_Field", buffer, 64, ImGuiInputTextFlags_EnterReturnsTrue)) {
				renameActive = false;
				target.renameEntity(buffer);
			}
			ImGui::SetKeyboardFocusHere(0);
			if (!ImGui::IsItemHovered() && ImGui::IsAnyMouseDown()) {
				renameActive = false;
			}
		}
		else {
			ImGui::Text(target.getEntityName().c_str());
			ImGui::SameLine();
			if (ImGui::Button("Rename")) {
				renameActive = true;
				renameValue = target.getEntityName();
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
			ImGui::Text("Are you sure you want to delete %s?\n\n", target.getEntityName().c_str());
			ImGui::Separator();

			if (ImGui::Button("Delete", ImVec2(120, 0))) {
				if (Scene * scene = target.getScene()) {
					scene->destroyEntity(target.getEntity());
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
		ImGui::BeginChild(target.getEntityName().c_str(), ImVec2(0, 0));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		std::size_t i = 0;
		for (Component* component : target.getComponents()) {
			renderComponent(component, i++);
		}
		if (ImGui::Button("Add component", ImVec2(ImGui::GetContentRegionAvailWidth(), 30)))
			ImGui::OpenPopup("add_component_popup");
		if (ImGui::BeginPopup("add_component_popup")) {
			std::vector<ComponentTypeID> filter;
			for (Component* component : target.getComponents()) {
				filter.push_back(component->getType().typeID);
			}
			renderComponentCombo(target, filter, Mirror::ReflectedTypes{});
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