#include "Hierarchy.h"
#include "utils/string.h"
#include "LevelEditor.h"
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <Core/Core.h>
#include <Core/Scene.h>
#include <Core/EntityHandle.h>
#include <Core/SceneManager.h>


using namespace Editor;
using namespace Core;

void sceneAddedCallback(void* ptr, IScene* scene) {
	static_cast<Hierarchy*>(ptr)->onSceneAdded(scene);
}

void sceneRemovedCallback(void* ptr, IScene* scene) {
	static_cast<Hierarchy*>(ptr)->onSceneRemoved(scene);
}

IEntityHandle* getEntityHandle(const std::vector<IEntityHandle*>& handles, Entity entity) {
	for (IEntityHandle* handle : handles) {
		if (handle->getEntity() == entity) {
			return handle;
		}
	}
	std::cout << "Hierarchy::getEntityHandle::Error Failed to find match" << std::endl;
	return nullptr;
}

std::vector<IEntityHandle*> getAllEntitiesFromScene(IScene* scene) {
	std::size_t count = scene->getAllEntitiesCount();
	std::vector<IEntityHandle*> allEntities(count);
	scene->getAllIEntities(&allEntities[0], count);
	return allEntities;
}

void entityNode(EntityHierarchy& entry, IEntityHandle* target, GameView* gameView) {
	IEntityHandle* entity = entry.entity;
	std::string name = entity->getEntityName();
	std::size_t childCount = entity->getChildCount();
	bool selected = target && target->isValid() && entity == target;
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
	if (selected) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if (childCount == 0) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), flags);
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		gameView->setTarget(entity);
	}
	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &entity->getEntity(), sizeof(Entity));
		ImGui::Text("%s", name.c_str());
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY")) {
			IM_ASSERT(payload->DataSize == sizeof(Entity));
			Entity* data = static_cast<Entity*>(payload->Data);

			IEntityHandle* dataHandle = getEntityHandle(getAllEntitiesFromScene(entity->getIScene()), *data);

			if (dataHandle && !dataHandle->isChild(entity->getEntity())) {
				dataHandle->setParent(entity->getEntity(), true);
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (nodeOpen) {
		for (std::size_t i = 0; i < entry.children.size(); i++) {
			EntityHierarchy& child = entry.children[i];
			entityNode(child, target, gameView);
		}
		ImGui::TreePop();
	}
}

Hierarchy::Hierarchy(LevelEditor* editor, GameView* gameView) : editor(editor), gameView(gameView) {
}
Hierarchy::~Hierarchy() {}

void Hierarchy::tick(IEntityHandle* target) {
	IEngine* engine = editor->getEngine();
	update();
	ImGui::Begin("Hierarchy");
	for (auto& scenePair : sceneOrder) {
		bool openPopup = false;
		IScene* scene = scenePair.first;
		std::string sceneName = utf8_encode(scene->getName());
		std::string scenePopupId = std::string("scene_popup_").append(sceneName);
		bool nodeOpened = ImGui::TreeNodeEx(sceneName.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			openPopup = true;
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY")) {
				IM_ASSERT(payload->DataSize == sizeof(Entity));
				Entity* data = static_cast<Entity*>(payload->Data);
				IEntityHandle* dataHandle = getEntityHandle(getAllEntitiesFromScene(scene), *data);

				if (dataHandle)
					dataHandle->removeParent(true);
			}
			ImGui::EndDragDropTarget();
		}
		if (nodeOpened) {
			for (EntityHierarchy& entity : scenePair.second) {
				entityNode(entity, target, gameView);
			}
			ImGui::TreePop();
		}
		if (openPopup) {
			ImGui::OpenPopup(scenePopupId.c_str());
		}
		if (ImGui::BeginPopup(scenePopupId.c_str())) {
			if (ImGui::Selectable("New Entity (WIP)", false)) {
				ImGui::OpenPopup("create_entity_popup"); // TODO: WIP. CURRENTLY DOES NOT WORK. Target this scene
			}
			if (ImGui::Selectable("Save and Close", false)) {
				if (engine) {
					engine->getSceneManagerInterface()->saveScene(scene);
				}
				editor->closeScene(scene);
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

std::unordered_map<IScene*, std::vector<IEntityHandle*>>::iterator findSceneOrder(IScene* scene, std::unordered_map<IScene*, std::vector<IEntityHandle*>>& sceneOrder) {
	for (auto it = sceneOrder.begin(); it != sceneOrder.end(); it++) {
		if (it->first == scene) return it;
	}
	return sceneOrder.end();
}

std::size_t Hierarchy::getRootIndex(IScene* scene, const IEntityHandle* handle) {
	/*auto it = findSceneOrder(scene, sceneOrder);
	if (it == sceneOrder.end()) {
		std::cout << "Hierarchy::getRootIndex::ERROR The Scene does not have any order.";
		return 0;
	}
	std::vector<IEntityHandle*>& roots = it->second;
	auto entityIt = std::find(roots.begin(), roots.end(), handle);
	if (entityIt == roots.end()) {
		return roots.size() - 1;
	}
	return entityIt - roots.begin();*/
	return 0;
}

bool Hierarchy::setRootIndex(IScene* scene, IEntityHandle* handle, std::size_t index) {
	/*auto it = findSceneOrder(scene, sceneOrder);
	if (it == sceneOrder.end()) return false;
	std::vector<IEntityHandle*>& roots = it->second;
	if (index >= roots.size()) index = roots.size();
	auto entityIt = std::find(roots.begin(), roots.end(), handle);
	if (entityIt != roots.end()) {
		auto eraseIt = roots.erase(entityIt);
		if (index > eraseIt - roots.begin()) index--;
	}
	roots.insert(roots.begin() + index, handle);
	return true;*/
	return true;
}

std::vector<IScene*> getNewScenes(const std::vector<IScene*>& scenes, const std::unordered_map<IScene*, std::vector<IEntityHandle*>>& sceneOrder) {
	std::vector<IScene*> newScenes;
	for (IScene* scene : scenes) {
		if (sceneOrder.find(scene) == sceneOrder.end()) {
			newScenes.push_back(scene);
		}
	}
	return newScenes;
}

void eraseRemovedScenes(const std::vector<IScene*>& scenes, std::unordered_map<IScene*, std::vector<IEntityHandle*>>& sceneOrder) {
	auto it = sceneOrder.begin();
	while (it != sceneOrder.end()) {
		bool found = false;
		for (const IScene* scene : scenes) {
			if (it->first == scene) {
				found = true;
				break;
			}
		}
		if (!found)
			it = sceneOrder.erase(it);
		else
			it++;
	}
}

std::vector<IEntityHandle*> getNewEntities(const std::vector<IEntityHandle*>& currEntities, const std::vector<IEntityHandle*>& prevEntities) {
	std::vector<IEntityHandle*> newEntities;
	for (IEntityHandle* current : currEntities) {
		bool found = false;
		for (const IEntityHandle* prev : prevEntities) {
			if (prev == current) {
				found = true;
				break;
			}
		}
		if (!found) {
			newEntities.push_back(current);
		}
	}
	return newEntities;
}

/*void eraseRemovedEntities(const std::vector<IEntityHandle*>& currEntities, std::vector<EntityHierarchy>& prevEntities) {
	auto it = prevEntities.begin();
	while (it != prevEntities.end()) {
		bool found = false;
		for (const IEntityHandle* current : currEntities) {
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
}*/

std::vector<IEntityHandle*> getRootEntities(const std::vector<IEntityHandle*>& handles) {
	// Filters out all with a parent
	std::vector<IEntityHandle*> roots;
	for (IEntityHandle* entity : handles) {
		if (!entity->hasParent())
			roots.push_back(entity);
	}
	return roots;
}

EntityHierarchy createEntityHierarchy(const std::vector<IEntityHandle*>& handles, IEntityHandle* handle) {
	std::size_t count = handle->getImmediateChildCount();
	std::vector<EntityHierarchy> children(count);
	for (std::size_t i = 0; i < count; i++) {
		IEntityHandle* child = getEntityHandle(handles, handle->getChildEntity(i));
		children[0] = createEntityHierarchy(handles, child);
	}
	return EntityHierarchy(handle, children);
}

std::vector<EntityHierarchy> getCurrentHierarchy(IScene* scene) {
	std::vector<IEntityHandle*> allEntities = getAllEntitiesFromScene(scene);
	// Create hierarchy
	std::vector<EntityHierarchy> hierarchy;
	for (IEntityHandle* handle : getRootEntities(allEntities)) {
		hierarchy.push_back(createEntityHierarchy(allEntities, handle));
	}
	return hierarchy;
}

void Hierarchy::update() {
	IEngine* engine = editor->getEngine();
	if (engine) {
		// Check for scene changes
		for (auto it = sceneOrder.begin(); it != sceneOrder.end(); it++) {
			if (it->first->hasEntitiesChanged()) {
				onSceneChanged(it);
			}
		}
	}
}

void Hierarchy::onSceneAdded(IScene* scene) {
	sceneOrder.insert(std::pair(scene, getCurrentHierarchy(scene)));
}

void Hierarchy::onSceneRemoved(IScene* scene) {
	auto it = sceneOrder.find(scene);
	if (it != sceneOrder.end()) {
		sceneOrder.erase(it);
	}
}

void Hierarchy::onSceneChanged(std::unordered_map<Core::IScene*, std::vector<EntityHierarchy>>::iterator& iterator) {
	iterator->second = getCurrentHierarchy(iterator->first);
}

void Hierarchy::initiate() {
	IEngine* engine = editor->getEngine();
	if (engine) {
		ISceneManager* sceneManager = engine->getSceneManagerInterface();
		sceneManager->setCallbackPtr(this);
		sceneManager->setSceneAddedCallback(sceneAddedCallback);
		sceneManager->setSceneRemovedCallback(sceneRemovedCallback);
	}
}

void Hierarchy::clear() {
	sceneOrder.clear();
}