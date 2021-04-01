#include "GameView.h"
#include "LevelEditor.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <Core/Core.h>
#include <Core/Camera.h>
#include <Core/Window.h>
#include <Core/Engine.h>
#include <GLFW/glfw3.h>

using namespace Core;
using namespace Editor;


GameView::GameView(LevelEditor* editor) : editor(editor), target(nullptr) {
}

GameView::~GameView() {
	if (target)
		target->release();
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
	DLLInterface* dllInterface = engineDLL->getInterface();
	if (!dllInterface) {
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
	IEngine* engine = editor->getEngine();
	ICamera* camera = engine->getGraphicsInterface()->getCameraInterface();
	IWindow* window = engine->getGraphicsInterface()->getWindowInterface();

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
	if (target && dllInterface->hasTransform(target)) {
		Core::Vec2 targetSize = dllInterface->getSize(target);
		Core::Vec2 minPosition = dllInterface->getMinRectScreenPosition(camera, target);
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
				releaseTarget();
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

		if (targetPressed && target && target->isValid()) {
			Core::Vec2 position = dllInterface->getLocalPosition(target);
			position.x += dragDelta.x;
			position.y += dragDelta.y;
			dllInterface->setLocalPosition(target, position);
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
	EngineDLL* engineDLL = editor->getEngineDLL();
	auto dllInterface = engineDLL->getInterface();
	if (!dllInterface) {
		std::cout << "Failed to set target because no Engine DLL was loaded" << std::endl;
		return;
	}
	releaseTarget();
	target = dllInterface->createEntityHandle(handle->getIScene(), handle->getEntity());
	targetPressed = false;
}

ImVec2 GameView::getViewportSize() const {
	return viewportSize;
}

void GameView::releaseTarget() {
	if (target) {
		target->release();
	}
	target = nullptr;
}