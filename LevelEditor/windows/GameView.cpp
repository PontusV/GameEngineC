#include "GameView.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "graphics/Camera.h"
#include "engine/Engine.h"
#include "maths/MatrixTransform.h"
#include "maths/Vector2.h"

using namespace Core;
using namespace Editor;


GameView::GameView(Engine* engine) : engine(engine) {
}

GameView::~GameView() {
}

void GameView::initialize() {
	viewportSize = ImVec2(1.0f, 1.0f);
	viewport.initialize(viewportSize.x, viewportSize.y);
}

void GameView::tick(float deltaTime) {
	Camera& camera = engine->getGraphics().getCamera();

	ImGui::Begin("Scene window");

	ImVec2 pMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y);
	ImVec2 pMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y);

	ImVec2 currentViewportSize = ImVec2(pMax.x - pMin.x, pMax.y - pMin.y);
	if (viewportSize.x != currentViewportSize.x || viewportSize.y != currentViewportSize.y) {
		viewportSize = currentViewportSize;
		viewport.setSize(viewportSize.x, viewportSize.y);
		engine->resizeViewport(viewportSize.x, viewportSize.y);
	}

	// Game tick
	viewport.begin();
	engine->tick(deltaTime);
	viewport.end();

	// Grid background

	// Scene
	ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewport.getTextureID()), pMin, pMax, ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(pMin, pMax)) {
		pressed = true;
		ImVec2 mousePos = ImGui::GetMousePos();
		Vector2 position = Vector2(mousePos.x - pMin.x, mousePos.y - pMin.y);
		target = engine->getInput().getEntityAtPos(position.x, position.y);
	}

	if (pressed && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		pressed = false;
	}

	if (pressed && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		ImVec2 dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
		ImGui::ResetMouseDragDelta();

		if (target.isValid()) {
			if (RectTransform* transform = target.getComponent<RectTransform>()) {
				transform->moveX(dragDelta.x);
				transform->moveY(dragDelta.y);
			}
		}
		else {
			camera.moveX(-dragDelta.x);
			camera.moveY(-dragDelta.y);
		}
	}

	ImGui::End();
}
EntityHandle GameView::getTarget() {
	return target;
}