#include "Menubar.h"
#include "LevelEditor.h"
#include "utils/file.h"
#include "imgui/imgui.h"
using namespace Editor;


Menubar::Menubar() {}
Menubar::~Menubar() {}

void Menubar::tick(LevelEditor* editor, bool editMode) {
	ProjectSettings* projectSettings = editor->getProjectSettings();
	EngineDLL* engineDLL = editor->getEngineDLL();
	UndoRedoManager* undoRedoManager = editor->getUndoRedoManager();
	GameView* gameView = editor->getGameView();
	FileView* fileView = editor->getFileView();
	PopupManager* popupManager = editor->getPopupManager();

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("New", editMode)) {
				if (ImGui::MenuItem("New Project")) {
					popupManager->openCreateProject();
				}
				if (ImGui::MenuItem("New Scene", nullptr, nullptr, engineDLL->isLoaded() && projectSettings->isLoaded())) {
					popupManager->openCreateScene();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Open", editMode)) {
				if (ImGui::MenuItem("Open Project")) {
					std::wstring filePath = getOpenFileName(L"Select A File", L"All Project Files\0*.proj;\0", 1);

					if (!filePath.empty()) {
						editor->openProject(filePath);
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save Project", "Ctrl+S", nullptr, editMode && projectSettings->isLoaded())) { // TODO: Keybind
				projectSettings->save();
			}
			if (ImGui::MenuItem("Save All", "Ctrl+S", nullptr, editMode && engineDLL->isLoaded() && projectSettings->isLoaded())) { // TODO: Keybind
				editor->saveAll();
			}
			if (ImGui::MenuItem("Reload", nullptr, nullptr, projectSettings->isLoaded() && engineDLL->isLoaded())) {
				std::wstring path = editor->copyEngineDLL();
				if (!path.empty()) {
					editor->reloadEngine(path);
				}
			}
			if (ImGui::MenuItem("Build", nullptr, nullptr, editMode && projectSettings->isLoaded())) {
				editor->buildGame();
			}
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				editor->terminate();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "CTRL+Z", false, editMode && undoRedoManager->isUndoAvailable())) {
				undoRedoManager->undo();
				gameView->updateTargetData();
			}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, editMode && undoRedoManager->isRedoAvailable())) {
				undoRedoManager->redo();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {
				fileView->cut();
			}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {
				fileView->copy();
			}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {
				fileView->paste();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game Object")) {
			if (ImGui::MenuItem("Create new")) {
				popupManager->openCreateEntity();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}