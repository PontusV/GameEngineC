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

	grid.initialize(viewportSize.x, viewportSize.y, 100);
}

void GameView::tick(float deltaTime) {
	Camera& camera = engine->getGraphics().getCamera();
	Window& window = engine->getGraphics().getWindow();

	ImGui::Begin("Scene window");

	ImVec2 pMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y);
	ImVec2 pMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y);

	ImVec2 currentViewportSize = ImVec2(pMax.x - pMin.x, pMax.y - pMin.y);
	if (viewportSize.x != currentViewportSize.x || viewportSize.y != currentViewportSize.y) {
		viewportSize = currentViewportSize;
		viewport.setSize(viewportSize.x, viewportSize.y);
		grid.initialize(viewportSize.x, viewportSize.y, 100);
		engine->resizeViewport(viewportSize.x, viewportSize.y);
	}

	// Game tick
	viewport.begin();
	window.clear();
	grid.render(camera.getPosition().x, camera.getPosition().y);
	engine->tick(deltaTime);
	viewport.end();

	// Scene
	ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewport.getTextureID()), pMin, pMax, ImVec2(0, 1), ImVec2(1, 0));

	// Target rect
	if (RectTransform* transform = target.getComponent<RectTransform>()) {
		const Vector2& targetSize = transform->getSize();
		Vector2 position = camera.getViewMatrix() * transform->getPosition();
		Vector2 minPosition = Vector2(position.x + transform->getRectOffset().x, position.y + transform->getRectOffset().y);
		ImVec2 rectMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x + minPosition.x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y + minPosition.y);
		ImGui::GetWindowDrawList()->AddRect(rectMin, ImVec2(rectMin.x + targetSize.x, rectMin.y + targetSize.y), IM_COL32_WHITE, 0.0f, ImDrawCornerFlags_None, 2.0f);
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(pMin, pMax)) {
		pressed = true;
		ImVec2 mousePos = ImGui::GetMousePos();
		Vector2 position = Vector2(mousePos.x - pMin.x, mousePos.y - pMin.y);
		EntityHandle entity = engine->getInput().getEntityAtPos(position.x, position.y);
		if (entity.isValid()) {
			targetPressed = true;
			target = entity; // New target
		}
	}

	if (pressed && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		pressed = false;
		targetPressed = false;
	}

	if (pressed && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		ImVec2 dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
		ImGui::ResetMouseDragDelta();

		if (targetPressed && target.isValid()) {
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