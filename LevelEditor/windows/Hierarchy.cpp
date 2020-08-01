#include "Hierarchy.h"
#include "scene/Scene.h"
#include "entity/handle/Handle.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace Editor;
using namespace Core;

void entityNode(Handle& entity, Handle& target) {
	std::string name = entity.getEntityName().c_str();
	std::size_t childCount = entity.getChildCount();
	bool selected = target.isValid() && entity == target;
	if (childCount == 0) {
		ImGui::Selectable(name.c_str(), &selected);
	}
	bool nodeOpen = childCount == 0 ? false : ImGui::TreeNodeEx(name.c_str(), selected ? ImGuiTreeNodeFlags_Selected : 0);
	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &entity, sizeof(Handle));
		ImGui::Text("%s", name.c_str());
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
		{
			IM_ASSERT(payload->DataSize == sizeof(Handle));
			Handle* data = (Handle*)payload->Data;

			data->setParent(entity);
		}
		ImGui::EndDragDropTarget();
	}
	if (nodeOpen) {
		for (std::size_t i = 0; i < entity.getChildCount(); i++) {
			Handle child = entity.getChild(i);
			entityNode(child, target);
		}
		ImGui::TreePop();
	}
}

Hierarchy::Hierarchy(Core::SceneManager* sceneManager) : sceneManager(sceneManager) {}
Hierarchy::~Hierarchy() {}

void Hierarchy::tick(Handle& target) {
	update(); // TODO: Update every 0.1 sec
	ImGui::Begin("Hierarchy");
	for (auto& scenePair : sceneOrder) {
		ScenePtr scene = scenePair.first;
		bool nodeOpened = ImGui::TreeNode(scene->getName().c_str());
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY")) {
				IM_ASSERT(payload->DataSize == sizeof(Handle));
				Handle* data = (Handle*)payload->Data;

				data->removeParent();
			}
			ImGui::EndDragDropTarget();
		}
		if (nodeOpened) {
			for (Handle& entity : scene->getRootEntities()) {
				entityNode(entity, target);
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

std::unordered_map<ScenePtr, std::vector<Handle>>::iterator findSceneOrder(Scene* scene, std::unordered_map<ScenePtr, std::vector<Handle>>& sceneOrder) {
	for (auto it = sceneOrder.begin(); it != sceneOrder.end(); it++) {
		if (it->first.get() == scene) return it;
	}
	return sceneOrder.end();
}

std::size_t Hierarchy::getRootIndex(Scene* scene, const Handle& handle) {
	auto it = findSceneOrder(scene, sceneOrder);
	if (it == sceneOrder.end()) {
		std::cout << "Hierarchy::getRootIndex::ERROR The Scene does not have any order.";
		return 0;
	}
	std::vector<Handle>& roots = it->second;
	auto entityIt = std::find(roots.begin(), roots.end(), handle);
	if (entityIt == roots.end()) {
		return roots.size() - 1;
	}
	return entityIt - roots.begin();
}

bool Hierarchy::setRootIndex(Scene* scene, const Handle& handle, std::size_t index) {
	auto it = findSceneOrder(scene, sceneOrder);
	if (it == sceneOrder.end()) return false;
	std::vector<Handle>& roots = it->second;
	if (index >= roots.size()) index = roots.size();
	auto entityIt = std::find(roots.begin(), roots.end(), handle);
	if (entityIt != roots.end()) {
		auto eraseIt = roots.erase(entityIt);
		if (index > eraseIt - roots.begin()) index--;
	}
	roots.insert(roots.begin() + index, handle);
	return true;
}

std::vector<ScenePtr> getNewScenes(const std::vector<ScenePtr>& scenes, const std::unordered_map<ScenePtr, std::vector<Handle>>& sceneOrder) {
	std::vector<ScenePtr> newScenes;
	for (const ScenePtr& scene : scenes) {
		if (sceneOrder.find(scene) == sceneOrder.end()) {
			newScenes.push_back(scene);
		}
	}
	return newScenes;
}

void eraseRemovedScenes(const std::vector<ScenePtr>& scenes, std::unordered_map<ScenePtr, std::vector<Handle>>& sceneOrder) {
	auto it = sceneOrder.begin();
	while (it != sceneOrder.end()) {
		bool found = false;
		for (const ScenePtr& scene : scenes) {
			if (it->first == scene) {
				found = true;
				break;
			}
		}
		if (!found)
			sceneOrder.erase(it);
		else
			it++;
	}
}

std::vector<Handle> getNewEntities(const std::vector<Handle>& currEntities, const std::vector<Handle>& prevEntities) {
	std::vector<Handle> newEntities;
	for (const Handle& current : currEntities) {
		bool found = false;
		for (const Handle& prev : prevEntities) {
			if (prev == current) {
				found = true;
				break;
			}
		}
		if (found) {
			newEntities.push_back(current);
		}
	}
	return newEntities;
}

void eraseRemovedEntities(const std::vector<Handle>& currEntities, std::vector<Handle>& prevEntities) {
	auto it = prevEntities.begin();
	while (it != prevEntities.end()) {
		bool found = false;
		for (const Handle& current : currEntities) {
			if (*it == current) {
				found = true;
				break;
			}
		}
		if (!found)
			prevEntities.erase(it);
		else
			it++;
	}
}

void Hierarchy::update() {
	std::vector<ScenePtr> scenes = sceneManager->getAllScenes();

	// Remove scenes
	eraseRemovedScenes(scenes, sceneOrder);

	// Entity
	for (auto sceneIt = sceneOrder.begin(); sceneIt != sceneOrder.end(); sceneIt++) {
		std::vector<Handle>& prevEntities = sceneIt->second;
		std::vector<Handle> currEntities = sceneIt->first->getRootEntities();

		eraseRemovedEntities(currEntities, prevEntities);

		std::vector<Handle> newEntities = getNewEntities(currEntities, prevEntities);
		prevEntities.insert(prevEntities.end(), newEntities.begin(), newEntities.end());
	}

	// Add scenes
	for (ScenePtr scene : getNewScenes(scenes, sceneOrder)) {
		sceneOrder.insert(std::pair(scene, scene->getRootEntities()));
	}
}