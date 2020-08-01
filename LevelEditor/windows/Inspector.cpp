#include "Inspector.h"
#include "imgui/imgui.h"
#include "ReflectionPolymorph.generated.h"

using namespace Editor;
using namespace Core;

//-------------
#include <Windows.h>

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

#include <commdlg.h>

std::wstring getOpenFileName(const wchar_t* title, const wchar_t* filter, unsigned long filterCount) {

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	wchar_t f1[MAX_PATH] = { 0 };
	ZeroMemory(&f1, sizeof(f1));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = filterCount;
	ofn.lpstrFile = (wchar_t*)f1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) {
		return std::wstring(f1);
	}
	return std::wstring();
}

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

void Inspector::tick() {
	EntityHandle target = gameView->getTarget();

	ImGui::Begin("Inspector");

	if (target.isValid()) {
		ImGui::Text(target.getEntityName().c_str());
		ImGui::BeginChild(target.getEntityName().c_str(), ImVec2(0, 0));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		std::size_t i = 0;
		for (Component* component : target.getComponents()) {
			Mirror::Class type = component->getType();
			if (type.hasAnnotation("hideInInspector")) return;

			if (ImGui::CollapsingHeader(type.name.c_str())) {
				for (Mirror::Property& prop : type.properties) {

					void* ptr = Mirror::getPointer(prop, component, type.typeID);
					std::string labelString = prop.name + "##" + std::to_string(i++);
					const char* label = labelString.c_str();

					if (prop.type.isBool()) {
						ImGui::Checkbox(label, (bool*)ptr);
					} else if (prop.type.isNumber()) {
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
						if (ImGui::ColorEdit4(label, (float*)&color)) {
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
			}
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
	else {
		ImGui::Text("No target");
	}

	ImGui::End();
}