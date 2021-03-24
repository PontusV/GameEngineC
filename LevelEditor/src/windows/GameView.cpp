#include "GameView.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <Core/Core.h>
#include <Core/Camera.h>
#include <Core/Window.h>
#include <Core/Engine.h>
//#include "maths/MatrixTransform.h"		// TODO: DLL
//#include "maths/Vector2.h"				// TODO: DLL
//#include "components/RectTransform.h"	// TODO: DLL

using namespace Core;
using namespace Editor;


GameView::GameView(IEngine* engine) : engine(engine), target(Core::createEntityHandle(nullptr, Entity(0))) {
}

GameView::~GameView() {
	if (target)
		target->release();
}

void GameView::initialize() {
	viewportSize = ImVec2(1.0f, 1.0f);
	viewport.initialize(viewportSize.x, viewportSize.y);

	grid.initialize(viewportSize.x, viewportSize.y, 100);
}

void GameView::tick(float deltaTime) {
	ICamera* camera = engine->getGraphicsInterface()->getCameraInterface();
	IWindow* window = engine->getGraphicsInterface()->getWindowInterface();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
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
	window->clear();
	grid.render(camera->getX(), camera->getY());
	engine->tick(deltaTime);
	viewport.end();

	// Scene
	ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewport.getTextureID()), pMin, pMax, ImVec2(0, 1), ImVec2(1, 0));

	// Target rect
	if (Core::hasTransform(target)) {
		Core::Vec2 targetSize = Core::getSize(target);
		Core::Vec2 minPosition = Core::getMinRectScreenPosition(camera, target);
		ImVec2 rectMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x + minPosition.x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y + minPosition.y);
		ImGui::GetWindowDrawList()->AddRect(rectMin, ImVec2(rectMin.x + targetSize.x, rectMin.y + targetSize.y), IM_COL32_WHITE, 0.0f, ImDrawCornerFlags_None, 2.0f);
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(pMin, pMax) && ImGui::IsWindowHovered()) {
		pressed = true;
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 position = ImVec2(mousePos.x - pMin.x, mousePos.y - pMin.y);
		if (!ImGui::GetIO().KeyShift) {
			IEntityHandle* entity = engine->getInputInterface()->createEntityHandleAtPos(position.x, position.y);
			if (entity) {
				targetPressed = true;
				target->release();
				target = entity; // New target
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

		if (targetPressed && target->isValid()) {
			Core::Vec2 position = Core::getLocalPosition(target);
			position.x += dragDelta.x;
			position.y += dragDelta.y;
			Core::setLocalPosition(target, position);
		}
		else {
			camera->moveX(-dragDelta.x);
			camera->moveY(-dragDelta.y);
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}
IEntityHandle* GameView::getTarget() {
	return target;
}

void GameView::setTarget(Core::IEntityHandle* handle) {
	target->release();
	target = Core::createEntityHandle(handle->getIScene(), handle->getEntity());
	targetPressed = false;
}

ImVec2 GameView::getViewportSize() const {
	return viewportSize;
}