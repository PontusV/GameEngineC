#include "Toolbar.h"
#include "LevelEditor.h"
#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"
using namespace Editor;

Toolbar::Toolbar() {}
Toolbar::~Toolbar() {}

void Toolbar::tick(LevelEditor* editor) {
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	//ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight));
	//ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolbarSize));
	//ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_NoResize
		//| ImGuiWindowFlags_NoDocking
		//| ImGuiWindowFlags_NoTitleBar
		//| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		//| ImGuiWindowFlags_NoSavedSettings
		;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::Begin("TOOLBAR", NULL, window_flags);
	ImGui::PopStyleVar();

	if (editor->isInEditMode()) {
		if (ImGui::Button(ICON_FA_PLAY, ImVec2(0, 37))) {
			editor->setEditMode(false);
			editor->setPaused(false);
		}
		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		if (ImGui::Button(ICON_FA_STOP, ImVec2(0, 37))) {
			editor->setEditMode(true);
			editor->setPaused(false);
		}
		ImGui::PopStyleVar();
	}
	else {
		if (editor->isPaused()) {
			if (ImGui::Button(ICON_FA_PLAY, ImVec2(0, 37))) {
				editor->setEditMode(false);
				editor->setPaused(false);
			}
		}
		else {
			if (ImGui::Button(ICON_FA_PAUSE, ImVec2(0, 37))) {
				editor->setPaused(true);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_STOP, ImVec2(0, 37))) {
			editor->setEditMode(true);
			editor->setPaused(false);
		}
	}

	ImGui::End();
}