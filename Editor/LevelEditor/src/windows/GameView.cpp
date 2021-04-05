#include "GameView.h"
#include "LevelEditor.h"
#include "EngineDLL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

using namespace Editor;


GameView::GameView(LevelEditor* editor) : editor(editor), target({ 0, 0, std::string() }) {
}

GameView::~GameView() {
}

void GameView::initialize(ImVec2 viewportSize) {
	viewport.initialize(viewportSize.x, viewportSize.y);
	grid.initialize(viewportSize.x, viewportSize.y, 100);
}

void GameView::tick(float deltaTime) {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene window");

	ImVec2 pMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y);
	ImVec2 pMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y);

	EngineDLL* engineDLL = editor->getEngineDLL();
	if (!engineDLL->isLoaded()) {
		viewport.begin();
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		grid.render(0, 0);
		viewport.end();

		ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewport.getTextureID()), pMin, pMax, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();
		return;
	}

	ImVec2 currentViewportSize = ImVec2(pMax.x - pMin.x, pMax.y - pMin.y);
	if (viewportSize.x != currentViewportSize.x || viewportSize.y != currentViewportSize.y) {
		viewportSize = currentViewportSize;
		viewport.setSize(viewportSize.x, viewportSize.y);
		grid.initialize(viewportSize.x, viewportSize.y, 100);
		engineDLL->setViewportSize(viewportSize.x, viewportSize.y);
	}
	ImVec2 cameraPosition = engineDLL->getCameraPosition();

	// Game tick
	viewport.begin();
	glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	grid.render(cameraPosition.x, cameraPosition.y);
	engineDLL->engineTick(deltaTime);
	viewport.end();

	// Scene
	ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewport.getTextureID()), pMin, pMax, ImVec2(0, 1), ImVec2(1, 0));

	// Target rect
	if (target.entityID != 0) {
		ImVec2 targetSize = engineDLL->getRectSize(target.entityID);
		ImVec2 minPosition = engineDLL->getMinRectScreenPosition(target.entityID);
		ImVec2 rectMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x + minPosition.x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y + minPosition.y);
		ImGui::GetWindowDrawList()->AddRect(rectMin, ImVec2(rectMin.x + targetSize.x, rectMin.y + targetSize.y), IM_COL32_WHITE, 0.0f, ImDrawCornerFlags_None, 2.0f);
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(pMin, pMax) && ImGui::IsWindowHovered()) {
		pressed = true;
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 position = ImVec2(mousePos.x - pMin.x, mousePos.y - pMin.y);
		if (!ImGui::GetIO().KeyShift) {
			EntityID entityID = engineDLL->getEntityAtPos(position.x, position.y);
			if (entityID != 0) {
				targetPressed = true;
				releaseTarget();
				target.entityID = entityID; // New target
				target.sceneIndex = engineDLL->getEntitySceneIndex(entityID);
				target.entityName = engineDLL->getEntityName(entityID);
			}
		}
	}

	if (pressed && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		pressed = false;
		targetPressed = false;
	}

	if (pressed && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		ImVec2 dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
		ImGui::ResetMouseDragDelta();

		if (targetPressed && target.entityID != 0) {
			ImVec2 position = engineDLL->getLocalPosition(target.entityID);
			position.x += dragDelta.x;
			position.y += dragDelta.y;
			engineDLL->setLocalPosition(target.entityID, position.x, position.y);
		}
		else {
			ImVec2 cameraPosition = engineDLL->getCameraPosition();
			engineDLL->setCameraPosition(cameraPosition.x - dragDelta.x, cameraPosition.y - dragDelta.y);
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}
EntityTargetData GameView::getTarget() {
	return target;
}

void GameView::setTarget(EntityID entityID) {
	EngineDLL* engineDLL = editor->getEngineDLL();
	target.entityID = entityID;
	target.sceneIndex = engineDLL->getEntitySceneIndex(entityID);
	target.entityName = engineDLL->getEntityName(entityID);
	targetPressed = false;
}

void GameView::updateTargetData() {
	EngineDLL* engineDLL = editor->getEngineDLL();
	EntityID entityID = target.entityID;
	target.sceneIndex = engineDLL->getEntitySceneIndex(entityID);
	target.entityName = engineDLL->getEntityName(entityID);
}

ImVec2 GameView::getViewportSize() const {
	return viewportSize;
}

void GameView::releaseTarget() {
	target.entityID = 0;
	target.sceneIndex = 0;
	target.entityName = std::string();
}