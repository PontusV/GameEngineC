#include "GameView.h"
#include "LevelEditor.h"
#include "EngineDLL.h"
#include "UndoRedo.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

using namespace Editor;


GameView::GameView(LevelEditor* editor, UndoRedoManager* undoRedoManager) : editor(editor), undoRedoManager(undoRedoManager), target({ 0, 0, std::string() }) {
}

GameView::~GameView() {
}

void GameView::initialize(ImVec2 viewportSize) {
	viewport.initialize(viewportSize.x, viewportSize.y);
	viewportBG.initialize(viewportSize.x, viewportSize.y, true);
	grid.initialize(viewportSize.x, viewportSize.y, 100);
}

void GameView::tick(float deltaTime, std::size_t fpsCount) {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene window");

	EngineDLL* engineDLL = editor->getEngineDLL();
	ImVec2 pMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y);
	ImVec2 pMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y);
	if (!engineDLL->isLoaded()) {
		viewportBG.begin();
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		grid.render(0, 0);
		viewportBG.end();

		ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewport.getTextureID()), pMin, pMax, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();
		return;
	}
	ImVec2 cameraPosition = engineDLL->getCameraPosition();
	bool mouseOnWindow = ImGui::IsMouseHoveringRect(pMin, pMax) && ImGui::IsWindowHovered();

	if (mouseOnWindow) {
		float scrollY = ImGui::GetIO().MouseWheel;
		if (scrollY != 0) {
			float zoomDelta = -scrollY / 10;
			setZoom(zoom + zoomDelta);
			grid.setZoom(zoom);
			//engineDLL->setCameraPosition(cameraPosition.x, cameraPosition.y);
			// TODO: Move camera to zoom towards/backwards from mouse pos?
			//ImVec2 move = ImVec2((pMax.x - pMin.x) * zoomDelta / 2, (pMax.y - pMin.y) * zoomDelta / 2);
			//cameraPosition = ImVec2(cameraPosition.x - move.x, cameraPosition.y - move.y);
		}
	}

	ImVec2 currentViewportSize = ImVec2(pMax.x - pMin.x, pMax.y - pMin.y);
	if (viewportSize.x != currentViewportSize.x || viewportSize.y != currentViewportSize.y) {
		viewportSize = currentViewportSize;
		viewport.setSize(viewportSize.x, viewportSize.y);
		viewportBG.setSize(viewportSize.x, viewportSize.y, true);
		grid.initialize(viewportSize.x, viewportSize.y, 100);
		engineDLL->setViewportSize(viewportSize.x, viewportSize.y);
	}

	// Render background
	viewportBG.begin();
	glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	grid.render(cameraPosition.x, cameraPosition.y);
	viewportBG.end();
	// Game tick
	viewport.begin();
	glClearColor(0.0, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	engineDLL->engineTick(deltaTime);
	viewport.end();

	// Scene
	ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewportBG.getTextureID()), pMin, pMax, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewport.getTextureID()), pMin, pMax, ImVec2(0, 1), ImVec2(1, 0));

	// Target rect
	if (target.entityID != 0) {
		ImVec2 targetSize = engineDLL->getRectSize(target.entityID);
		ImVec2 minPosition = engineDLL->getMinRectScreenPosition(target.entityID);
		ImVec2 rectMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x + (minPosition.x), ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y + (minPosition.y));
		ImGui::GetWindowDrawList()->AddRect(rectMin, ImVec2(rectMin.x + (targetSize.x / zoom), rectMin.y + (targetSize.y / zoom)), IM_COL32_WHITE, 0.0f, ImDrawCornerFlags_None, 2.0f);
	}

	// Click target
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && mouseOnWindow) {
		pressed = true;
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 screenPosition = ImVec2(mousePos.x - pMin.x, mousePos.y - pMin.y);
		ImVec2 position = ImVec2(screenPosition.x, screenPosition.y);
		if (!ImGui::GetIO().KeyShift) {
			EntityID entityID = engineDLL->getEntityAtPos(position.x, position.y);
			if (entityID != 0) {
				targetPressedPosition = engineDLL->getWorldPosition(entityID);
				targetPressed = true;
				releaseTarget();
				target.entityID = entityID; // New target
				target.sceneIndex = engineDLL->getEntitySceneIndex(entityID);
				target.entityName = engineDLL->getEntityName(entityID);
			}
		}
	}

	// Click release target
	if (pressed && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		pressed = false;
		targetPressed = false;
		if (draggingTarget) {
			undoRedoManager->registerUndo(std::make_unique<MoveEntityAction>(target.sceneIndex, target.entityName, targetPressedPosition.x, targetPressedPosition.y));
			draggingTarget = false;
		}
	}

	// Dragging target
	if (pressed && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 10.0f)) {
		ImVec2 dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
		ImGui::ResetMouseDragDelta();

		if (targetPressed && target.entityID != 0) {
			draggingTarget = true;
			ImVec2 position = engineDLL->getLocalPosition(target.entityID);
			position.x += dragDelta.x * zoom;
			position.y += dragDelta.y * zoom;
			engineDLL->setLocalPosition(target.entityID, position.x, position.y);
		}
		else {
			engineDLL->setCameraPosition(cameraPosition.x - (dragDelta.x * zoom), cameraPosition.y - (dragDelta.y * zoom));
		}
	}

	// UI
	ImGui::Text("(%d, %d)", (int)cameraPosition.x, (int)cameraPosition.y);
	ImGui::Text("FPS: %d", fpsCount);
	ImGui::Text("Zoom: %d %%", (int)(zoom * 100));

	ImGui::End();
	ImGui::PopStyleVar();
}
EntityTargetData GameView::getTarget() {
	return target;
}

void GameView::setTarget(EntityID entityID) {
	if (targetPressed) {
		targetPressed = false;
		pressed = false;
		if (draggingTarget) {
			undoRedoManager->registerUndo(std::make_unique<MoveEntityAction>(target.sceneIndex, target.entityName, targetPressedPosition.x, targetPressedPosition.y));
			draggingTarget = false;
		}
	}
	EngineDLL* engineDLL = editor->getEngineDLL();
	target.entityID = entityID;
	target.sceneIndex = engineDLL->getEntitySceneIndex(entityID);
	target.entityName = engineDLL->getEntityName(entityID);
}

void GameView::updateTargetData() {
	EngineDLL* engineDLL = editor->getEngineDLL();
	EntityID entityID = target.entityID;
	bool entityExists = false;
	for (EntityID& id : engineDLL->getAllEntities(entityID)) {
		if (id == entityID) {
			entityExists = true;
			break;
		}
	}
	if (entityExists) {
		target.sceneIndex = engineDLL->getEntitySceneIndex(entityID);
		target.entityName = engineDLL->getEntityName(entityID);
	}
	else {
		releaseTarget();
	}
}

void GameView::setZoom(float value) {
	if (value < 0.1f) return;
	zoom = std::floor(value * 10) / 10;
	editor->getEngineDLL()->setCameraScale(1.0f / zoom);
}

ImVec2 GameView::getViewportSize() const {
	return viewportSize;
}

void GameView::releaseTarget() {
	target.entityID = 0;
	target.sceneIndex = 0;
	target.entityName = std::string();
}