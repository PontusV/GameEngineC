#include "Hierarchy.h"
#include "utils/string.h"
#include "LevelEditor.h"
#include "EngineDLL.h"
#include "GameView.h"
#include "UndoRedo.h"
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "IconsFontAwesome5.h"


using namespace Editor;

void sceneAddedCallback(void* ptr, std::size_t sceneIndex) {
	static_cast<LevelEditor*>(ptr)->getHierarchy()->onSceneAdded(sceneIndex);
}

void sceneRemovedCallback(void* ptr, std::size_t sceneIndex) {
	static_cast<LevelEditor*>(ptr)->getHierarchy()->onSceneRemoved(sceneIndex);
}

void entityRenamedCallback(void* ptr, EntityID entityID) {
	static_cast<LevelEditor*>(ptr)->getHierarchy()->onEntityChanged(entityID);
}

void entityNode(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager, std::size_t sceneIndex, EntityHierarchy& entry, EntityID targetID, GameView* gameView) {
	EntityData entity = entry.entity;
	std::string name = entity.name;
	std::string label = ICON_FA_MALE + (" " + name);
	std::size_t childCount = entry.children.size();
	bool selected = entity.id == targetID;
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
	if (selected) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if (childCount == 0) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), flags, label.c_str());
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		gameView->setTarget(entity.id);
	}
	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &entity, sizeof(EntityData));
		ImGui::Text("%s", name.c_str());
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY")) {
			IM_ASSERT(payload->DataSize == sizeof(EntityData));
			EntityData* data = static_cast<EntityData*>(payload->Data);
			EntityID prevParentID = engineDLL->getEntityParent(data->id);
			if (engineDLL->setEntityParent(sceneIndex, data->id, entity.id)) {
				std::string prevParentName = engineDLL->getEntityName(prevParentID);
				undoRedoManager->registerUndo(std::make_unique<SetEntityParentAction>(sceneIndex, data->name, prevParentName));
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (nodeOpen) {
		for (std::size_t i = 0; i < entry.children.size(); i++) {
			EntityHierarchy& child = entry.children[i];
			entityNode(engineDLL, undoRedoManager, sceneIndex, child, targetID, gameView);
		}
		ImGui::TreePop();
	}
}

Hierarchy::Hierarchy(LevelEditor* editor, GameView* gameView, UndoRedoManager* undoRedoManager) : editor(editor), gameView(gameView), undoRedoManager(undoRedoManager) {
}
Hierarchy::~Hierarchy() {}

void Hierarchy::tick(EntityID target) {
	EngineDLL* engineDLL = editor->getEngineDLL();
	update();
	ImGui::Begin("Hierarchy");
	std::size_t sceneIndex = 0;
	for (SceneData& sceneData : sceneOrder) {
		bool openPopup = false;
		std::string sceneName = sceneData.name;
		std::string sceneLabel = ICON_FA_CUBE + (" " + sceneName);
		if (undoRedoManager->getStepsSinceSave(sceneIndex) != 0) {
			sceneLabel.append("*");
		}
		std::string scenePopupId = std::string("scene_popup_").append(sceneName);
		bool nodeOpened = ImGui::TreeNodeEx(sceneLabel.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			openPopup = true;
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY")) {
				IM_ASSERT(payload->DataSize == sizeof(Entity));
				EntityData* data = static_cast<EntityData*>(payload->Data);
				EntityID parentID = engineDLL->getEntityParent(data->id);
				if (engineDLL->detachEntityParent(sceneIndex, data->id)) {
					std::string prevParentName = engineDLL->getEntityName(parentID);
					undoRedoManager->registerUndo(std::make_unique<SetEntityParentAction>(sceneIndex, data->name, prevParentName));
				}
			}
			ImGui::EndDragDropTarget();
		}
		if (nodeOpened) {
			for (EntityHierarchy& entity : sceneData.roots) {
				entityNode(engineDLL, undoRedoManager, sceneIndex, entity, target, gameView);
			}
			ImGui::TreePop();
		}
		if (openPopup) {
			ImGui::OpenPopup(scenePopupId.c_str());
		}
		if (ImGui::BeginPopup(scenePopupId.c_str())) {
			if (ImGui::Selectable("Make Active scene", activeSceneIndex == sceneIndex)) {
				setActiveSceneIndex(sceneIndex);
			}
			if (ImGui::Selectable("New Entity", false)) {
				editor->getPopupManager()->openCreateEntity(sceneIndex);
			}
			if (ImGui::Selectable("Save", false)) {
				if (engineDLL->isLoaded()) {
					engineDLL->saveScene(sceneIndex);
				}
			}
			if (ImGui::Selectable("Save and Close", false)) {
				if (engineDLL->isLoaded()) {
					engineDLL->saveScene(sceneIndex);
				}
				editor->closeScene(sceneIndex);
			}
			ImGui::EndPopup();
		}
		sceneIndex++;
	}
	ImGui::End();
}

/*std::unordered_map<IScene*, std::vector<IEntityHandle*>>::iterator findSceneOrder(IScene* scene, std::unordered_map<IScene*, std::vector<IEntityHandle*>>& sceneOrder) {
	for (auto it = sceneOrder.begin(); it != sceneOrder.end(); it++) {
		if (it->first == scene) return it;
	}
	return sceneOrder.end();
}*/

std::size_t Hierarchy::getRootIndex(std::size_t sceneIndex, EntityID entityID) {
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

bool Hierarchy::setRootIndex(std::size_t sceneIndex, EntityID entityID, std::size_t index) {
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

std::vector<EntityData> getRootEntities(EngineDLL* engineDLL, const std::vector<EntityData>& entities) {
	// Filters out all with a parent
	std::vector<EntityData> roots;
	for (const EntityData& entity : entities) {
		if (!engineDLL->hasEntityParent(entity.id))
			roots.push_back(entity);
	}
	return roots;
}

EntityData getEntityData(const std::vector<EntityData>& entities, EntityID entityID) {
	for (const EntityData& entityData : entities) {
		if (entityData.id == entityID)
			return entityData;
	}
	return EntityData{ 0, std::string() };
}

EntityHierarchy createEntityHierarchy(EngineDLL* engineDLL, const std::vector<EntityData>& entities, EntityData entity) {
	std::size_t count = engineDLL->getEntityImmediateChildCount(entity.id);
	std::vector<EntityHierarchy> children(count);
	for (std::size_t i = 0; i < count; i++) {
		EntityID childID = engineDLL->getEntityChild(entity.id, i);
		EntityData child = getEntityData(entities, childID);
		children[i] = createEntityHierarchy(engineDLL, entities, child);
	}
	return EntityHierarchy(entity, children);
}

std::vector<EntityHierarchy> getCurrentHierarchy(EngineDLL* engineDLL, std::size_t sceneIndex) {
	std::vector<EntityID> allEntityIDs = engineDLL->getAllEntities(sceneIndex);
	std::vector<EntityData> allEntities(allEntityIDs.size());
	for (std::size_t i = 0; i < allEntityIDs.size(); i++) {
		EntityData& entityData = allEntities[i];
		entityData.id = allEntityIDs[i];
		entityData.name = engineDLL->getEntityName(entityData.id);
	}
	// Create hierarchy
	std::vector<EntityHierarchy> hierarchy;
	for (EntityData& entityData : getRootEntities(engineDLL, allEntities)) {
		hierarchy.push_back(createEntityHierarchy(engineDLL, allEntities, entityData));
	}
	return hierarchy;
}

void Hierarchy::update() {
	EngineDLL* engineDLL = editor->getEngineDLL();
	if (engineDLL->isLoaded()) {
		// Check for scene changes
		std::size_t sceneCount = engineDLL->getSceneCount();
		for (std::size_t i = 0; i < sceneCount; i++) {
			if (engineDLL->hasSceneChanged(i)) {
				onSceneChanged(i);
			}
		}
	}
}

bool updateEntityName(EngineDLL* engineDLL, std::vector<EntityHierarchy>& entities, EntityID entityID) {
	for (EntityHierarchy& data : entities) {
		if (data.entity.id == entityID) {
			data.entity.name = engineDLL->getEntityName(data.entity.id);
			return true;
		}
		if (updateEntityName(engineDLL, data.children, entityID)) {
			return true;
		}
	}
	return false;
}

void Hierarchy::onSceneAdded(std::size_t sceneIndex) {
	EngineDLL* engineDLL = editor->getEngineDLL();
	SceneData sceneData;
	sceneData.name = engineDLL->getSceneName(sceneIndex);
	sceneData.roots = getCurrentHierarchy(engineDLL, sceneIndex);
	sceneOrder.push_back(sceneData);
	gameView->updateTargetData();
}

void Hierarchy::onSceneRemoved(std::size_t sceneIndex) {
	if (sceneIndex >= sceneOrder.size()) {
		std::cout << "Hierarchy::onSceneRemoved::ERROR Out of bounds. Index: " << sceneIndex << ", size: " << sceneOrder.size() << std::endl;
		return;
	}
	sceneOrder.erase(sceneOrder.begin() + sceneIndex);
	if (gameView->getTarget().sceneIndex == sceneIndex) {
		gameView->releaseTarget();
	}
	else {
		gameView->updateTargetData();
	}
}

void Hierarchy::onSceneChanged(std::size_t sceneIndex) {
	if (sceneIndex > sceneOrder.size() - 1) return;
	EngineDLL* engineDLL = editor->getEngineDLL();
	SceneData& sceneData = sceneOrder[sceneIndex];
	sceneData.roots = getCurrentHierarchy(engineDLL, sceneIndex);
}

void Hierarchy::onEntityChanged(EntityID entityID) {
	EngineDLL* engineDLL = editor->getEngineDLL();
	std::size_t sceneIndex = engineDLL->getEntitySceneIndex(entityID);
	if (sceneOrder.size() - 1 >= sceneIndex) {
		if (!updateEntityName(engineDLL, sceneOrder[sceneIndex].roots, entityID)) {
			std::cout << "Hierarchy::onEntityChanged::ERROR Failed to update name of Entity with ID: " << entityID << std::endl;
		}
	}
}

void Hierarchy::initiate() {
	EngineDLL* engineDLL = editor->getEngineDLL();
	if (engineDLL->isLoaded()) {
		engineDLL->setSceneAddedCallback(sceneAddedCallback);
		engineDLL->setSceneRemovedCallback(sceneRemovedCallback);
		engineDLL->setEntityRenamedCallback(entityRenamedCallback);
	}
}

void Hierarchy::clear() {
	sceneOrder.clear();
}

std::size_t Hierarchy::getActiveSceneIndex() {
	return activeSceneIndex;
}

void Hierarchy::setActiveSceneIndex(std::size_t sceneIndex) {
	if (sceneOrder.size() <= sceneIndex) {
		std::cout << "Heirarchy::setACtiveSceneIndex::ERROR Unable to set activeSceneIndex to " << sceneIndex << ". Scene count: " << sceneOrder.size() << std::endl;
		return;
	}
	this->activeSceneIndex = sceneIndex;
}

std::size_t Hierarchy::getSceneIndexByName(std::string name) const {
	EngineDLL* engineDLL = editor->getEngineDLL();
	std::size_t sceneCount = engineDLL->getSceneCount();
	for (std::size_t i = 0; i < sceneCount; i++) {
		if (name.compare(engineDLL->getSceneName(i)) == 0) {
			return i;
		}
	}
	return -1;
}

std::size_t Hierarchy::getSceneCount() {
	return sceneOrder.size();
}

std::string Hierarchy::getSceneName(std::size_t sceneIndex) {
	if (sceneIndex >= sceneOrder.size()) {
		return "OUT_OF_BOUNDS";
	}
	return sceneOrder[sceneIndex].name;
}