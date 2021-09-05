#include "PopupManager.h"
#include "LevelEditor.h"
#include "imgui/imgui.h"
#include "utils/string.h"
#include "utils/file.h"
using namespace Editor;

constexpr wchar_t SCENE_FILE_TYPE[] = L".scene";

PopupManager::PopupManager() {}
PopupManager::~PopupManager() {}

void PopupManager::tick(LevelEditor* editor) {
	ProjectSettings& projectSettings = *editor->getProjectSettings();
	EngineDLL& engineDLL = *editor->getEngineDLL();
	UndoRedoManager& undoRedoManager = *editor->getUndoRedoManager();
	Hierarchy& hierarchy = *editor->getHierarchy();

	static std::wstring dirPath = L"";
	static bool chooseDirPath = false;

	if (open_create_scene_popup && engineDLL.isLoaded() && projectSettings.isLoaded()) {
		open_create_scene_popup = false;
		std::wstring path = getSaveFileName(L"Choose scene location", L"Scene\0*.scene;\0", 1, projectSettings.getPath().c_str());
		if (!path.empty()) {
			if (!path.ends_with(SCENE_FILE_TYPE)) {
				path.append(SCENE_FILE_TYPE);
			}
			std::size_t nameStartIndex = path.find_last_of(L"\\");
			std::wstring fileName = path.substr(nameStartIndex == std::wstring::npos ? 0 : nameStartIndex + 1);
			std::string encodedName = utf8_encode(fileName.substr(0, fileName.find_last_of(L".")));
			std::string encodedPath = utf8_encode(path);
			auto entityID = engineDLL.createPrefabEntity(encodedPath.c_str(), 0, 0);
			engineDLL.setEntityName(entityID, encodedName.c_str());
			hierarchy.setActiveScene(entityID);
			projectSettings.addOpenScene(path);
			projectSettings.save();
		}
	}

	if (open_create_project_popup) {
		open_create_project_popup = false;
		ImGui::OpenPopup("create_project_popup");
		chooseDirPath = true;
	}

	if (ImGui::BeginPopupModal("create_project_popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static std::string errorMessage = "";
		if (!errorMessage.empty()) {
			ImGui::TextColored(ImVec4(1, 0, 0, 1), errorMessage.c_str());
		}
		static char buffer[64] = "";
		if (ImGui::Button("Choose directory")) {
			chooseDirPath = true;
		}
		ImGui::Text(utf8_encode(dirPath).append(dirPath.ends_with(L"\\") ? "" : "\\").append(buffer).c_str());
		ImGui::InputText("Project name", buffer, 64);

		if (ImGui::Button("Create", ImVec2(120, 0))) {
			std::wstring name = utf8_decode(buffer);
			std::wstring path = std::wstring(dirPath).append(path.ends_with(L"/") || path.ends_with(L"\\") ? L"" : L"/").append(name);

			if (editor->createProject(name, path)) {
				editor->openProject(path);
				errorMessage = "";
				dirPath = L"";
				memset(buffer, 0, 64);
				ImGui::CloseCurrentPopup();
			}
			else {
				errorMessage = "Failed to create new project!";
			}
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			errorMessage = "";
			dirPath = L"";
			ImGui::CloseCurrentPopup();
		}
		if (chooseDirPath) {
			dirPath = getOpenFolderName(L"Choose project location");
			chooseDirPath = false;
			if (dirPath.empty()) {
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}

	if (open_create_entity_popup) {
		open_create_entity_popup = false;
		ImGui::OpenPopup("create_entity_popup");
	}

	if (ImGui::BeginPopupModal("create_entity_popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (engineDLL.isLoaded()) {
			static std::string errorMessage = "";
			if (!errorMessage.empty()) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), errorMessage.c_str());
			}
			static char buffer[64] = "";
			ImGui::InputText("Name", buffer, 64);

			if (ImGui::Button("Create", ImVec2(120, 0))) {
				EntityID parentID = createEntityParentID == 0 ? hierarchy.getActiveScene() : createEntityParentID;
				if (parentID != 0) {
					std::string name = std::string(buffer);
					ImVec2 cameraPosition = engineDLL.getCameraPosition();
					float width = 350;
					float height = 350;
					EntityID entityID = engineDLL.createTemplateEntity(name.c_str(), cameraPosition.x, cameraPosition.y, width, height);
					if (!engineDLL.setEntityParent(entityID, parentID)) {
						std::cout << "PopupManager::create_entity_popup::ERROR Failed to set Entity parent. Cleaning up..." << std::endl;
						if (!engineDLL.destroyEntity(entityID)) {
							std::cout << "PopupManager::create_entity_popup::ERROR Failed to cleanup Entity" << std::endl;
						}
					}
					EntityID rootEntityID = engineDLL.getRootEntityID(parentID);
					undoRedoManager.registerUndo(std::make_unique<DestroyEntityAction>(DestroyEntityAction(rootEntityID, entityID)));
					errorMessage = "";
					memset(buffer, 0, 64);
					ImGui::CloseCurrentPopup();
				}
				else {
					std::cout << "PopupManager::create_entity_popup::ERROR Failed to create Entity. No valid parent given" << std::endl;
				}
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				errorMessage = "";
				ImGui::CloseCurrentPopup();
			}
		}
		else {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (open_delete_entity_popup) {
		open_delete_entity_popup = false;
		ImGui::OpenPopup("Delete_entity");
	}

	if (ImGui::BeginPopupModal("Delete_entity", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		auto entityName = engineDLL.getEntityName(deleteEntityID);
		auto rootEntityID = engineDLL.getRootEntityID(deleteEntityID);
		EngineDLL* engineDLL = editor->getEngineDLL();
		UndoRedoManager* undoRedoManager = editor->getUndoRedoManager();
		GameView* gameView = editor->getGameView();
		ImGui::Text("Are you sure you want to delete %s?\n\n", entityName);
		ImGui::Separator();

		if (ImGui::Button("Delete", ImVec2(120, 0))) {
			if (deleteEntityID != 0) {
				auto serializedEntityData = engineDLL->writeEntityToBuffer(deleteEntityID);
				if (engineDLL->destroyEntity(deleteEntityID)) {
					undoRedoManager->registerUndo(std::make_unique<CreateEntityAction>(rootEntityID, deleteEntityID, std::move(serializedEntityData)));
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

	if (open_save_changes_popup) {
		open_save_changes_popup = false;
		ImGui::OpenPopup("Save_unsaved_changes");
	}

	if (ImGui::BeginPopupModal("Save_unsaved_changes", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		auto entityName = engineDLL.getEntityName(deleteEntityID);
		auto rootEntityID = engineDLL.getRootEntityID(deleteEntityID);
		EngineDLL* engineDLL = editor->getEngineDLL();
		UndoRedoManager* undoRedoManager = editor->getUndoRedoManager();
		GameView* gameView = editor->getGameView();
		Hierarchy* hierarchy = editor->getHierarchy();
		ImGui::Text("Save changes before exiting the application?");
		ImGui::Separator();

		std::size_t sceneCount = hierarchy->getSceneCount();
		for (std::size_t i = 0; i < sceneCount; i++) {
			std::string sceneName = engineDLL->getEntityName(rootEntityID);
			ImGui::Text("%s*", sceneName);
		}

		if (ImGui::Button("Save", ImVec2(120, 0))) {
			editor->saveAll();
			editor->terminate();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Don't save", ImVec2(120, 0))) {
			editor->terminate(true);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void PopupManager::openCreateScene() {
	open_create_scene_popup = true;
}

void PopupManager::openCreateProject() {
	open_create_project_popup = true;
}

void PopupManager::openCreateEntity(std::size_t sceneIndex) {
	open_create_entity_popup = true;
	createEntityParentID = sceneIndex;
}

void PopupManager::openDeleteEntity(std::size_t entityID) {
	open_delete_entity_popup = true;
	deleteEntityID = entityID;
}

void PopupManager::openSaveChanges() {
	open_save_changes_popup = true;
}