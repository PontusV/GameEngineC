#include "Hierarchy.h"
#include "utils/string.h"
#include "LevelEditor.h"
#include "EngineDLL.h"
#include "GameView.h"
#include "UndoRedo.h"
#include <iostream>
#include <algorithm>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "IconsFontAwesome5.h"


using namespace Editor;

void entitiesChangedCallback(void* ptr, std::size_t entityID) {
	static_cast<LevelEditor*>(ptr)->getHierarchy()->onEntitiesChanged(entityID);
}

void entityNode(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager, std::size_t rootEntityID, EntityHierarchyNode& node, EntityID targetID, GameView* gameView) {
	std::string name = node.name;
	std::string label = ICON_FA_MALE + (" " + name);
	std::size_t childCount = node.children.size();
	bool selected = node.entityID == targetID;
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
	if (selected) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if (childCount == 0) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), flags, label.c_str());
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		gameView->setTarget(node.entityID);
	}
	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &node.entityID, sizeof(EntityID));
		ImGui::Text("%s", name.c_str());
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY")) {
			IM_ASSERT(payload->DataSize == sizeof(EntityID));
			EntityID* data = static_cast<EntityID*>(payload->Data);
			EntityID& entityID = *data;
			EntityID prevParentID = engineDLL->getEntityParent(entityID);
			if (engineDLL->setEntityParent(entityID, node.entityID)) {
				undoRedoManager->registerUndo(std::make_unique<SetEntityParentAction>(rootEntityID, entityID, prevParentID));
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (nodeOpen) {
		for (std::size_t i = 0; i < node.children.size(); i++) {
			EntityHierarchyNode& child = node.children[i];
			entityNode(engineDLL, undoRedoManager, rootEntityID, child, targetID, gameView);
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
	for (EntityHierarchyRootNode& scene : scenes) {
		std::size_t& rootEntityID = scene.entityID;
		bool openPopup = false;
		std::string sceneName = scene.name;
		std::string sceneLabel = ICON_FA_CUBE + (" " + sceneName);
		if (undoRedoManager->getStepsSinceSave(rootEntityID) != 0) {
			sceneLabel.append("*");
		}
		std::string scenePopupId = std::string("scene_popup_").append(sceneName);
		bool nodeOpened = ImGui::TreeNodeEx(sceneLabel.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | (activeScene == rootEntityID ? ImGuiTreeNodeFlags_Selected : 0));
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			openPopup = true;
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY")) {
				IM_ASSERT(payload->DataSize == sizeof(EntityID));
				EntityID* data = static_cast<EntityID*>(payload->Data);
				EntityID& entityID = *data;
				EntityID parentID = engineDLL->getEntityParent(entityID);
				if (engineDLL->detachEntityParent(entityID)) {
					undoRedoManager->registerUndo(std::make_unique<SetEntityParentAction>(rootEntityID, entityID, parentID));
				}
			}
			ImGui::EndDragDropTarget();
		}
		if (nodeOpened) {
			for (EntityHierarchyNode& entity : scene.children) {
				entityNode(engineDLL, undoRedoManager, rootEntityID, entity, target, gameView);
			}
			ImGui::TreePop();
		}
		if (openPopup) {
			ImGui::OpenPopup(scenePopupId.c_str());
		}
		if (ImGui::BeginPopup(scenePopupId.c_str())) {
			if (ImGui::Selectable("Make Active scene", activeScene == rootEntityID)) {
				setActiveScene(rootEntityID);
			}
			if (ImGui::Selectable("New Entity", false)) {
				editor->getPopupManager()->openCreateEntity(rootEntityID);
			}
			if (ImGui::Selectable("Save", false)) {
				if (editor->isInEditMode() && engineDLL->isLoaded()) {
					engineDLL->savePrefab(scene.entityID, scene.filePath.c_str());
					engineDLL->updatePrefabs(scene.filePath.c_str());
					undoRedoManager->setDirty(rootEntityID, false);
					undoRedoManager->resetStepsSinceSave(rootEntityID);
				}
			}
			if (ImGui::Selectable("Save and Close", false)) {
				if (editor->isInEditMode()) {
					if (engineDLL->isLoaded()) {
						engineDLL->savePrefab(scene.entityID, scene.filePath.c_str());
						engineDLL->updatePrefabs(scene.filePath.c_str());
					}
					editor->closeScene(scene.entityID);
				}
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

EntityHierarchyNode createEntityHierarchyFrom(EngineDLL* engineDLL, EntityID entityID) {
	EntityHierarchyNode node;
	node.entityID = entityID;
	node.name = engineDLL->getEntityName(entityID);
	std::size_t count = engineDLL->getEntityImmediateChildCount(entityID);
	std::vector<EntityHierarchyNode>& children = node.children;
	children.reserve(count);
	for (std::size_t i = 0; i < count; i++) {
		EntityID childID = engineDLL->getEntityChild(entityID, i);
		children.push_back(createEntityHierarchyFrom(engineDLL, childID));
	}
	return node;
}

std::vector<EntityHierarchyRootNode> getScenes(EngineDLL* engineDLL) {
	// Filters out all with a parent
	auto entities = engineDLL->getAllEntities();
	std::vector<EntityHierarchyRootNode> scenes;
	for (const EntityID& entityID : entities) {
		if (!engineDLL->hasEntityParent(entityID)) {
			auto node = createEntityHierarchyFrom(engineDLL, entityID);
			EntityHierarchyRootNode& data = scenes.emplace_back();
			data.entityID = entityID;
			data.name = node.name;
			data.children = node.children;
			data.filePath = engineDLL->getPrefabFilePath(entityID);
		}
	}
	return scenes;
}

void Hierarchy::update() {
	/*EngineDLL* engineDLL = editor->getEngineDLL();
	if (engineDLL->isLoaded()) {
		// Check for scene changes
		std::size_t sceneCount = engineDLL->getSceneCount();
		for (std::size_t i = 0; i < sceneCount; i++) {
			if (engineDLL->hasSceneChanged(i)) {
				onSceneChanged(i);
			}
		}
	}*/
	if (isDirty()) {
		EngineDLL* engineDLL = editor->getEngineDLL();
		scenes = getScenes(engineDLL);
		gameView->updateTargetData();
		setDirty(false);
	}
}

void Hierarchy::onEntitiesChanged(EntityID entityID) {
	setDirty(true);
}

void Hierarchy::initiate() {
	EngineDLL* engineDLL = editor->getEngineDLL();
	if (engineDLL->isLoaded()) {
		engineDLL->setEntitiesChangedCallback(entitiesChangedCallback);
	}
}

void Hierarchy::clear() {
	scenes.clear();
}

bool Hierarchy::isDirty() const {
	return dirty;
}

void Hierarchy::setDirty(bool value) {
	dirty = value;
}

EntityID Hierarchy::getActiveScene() {
	return activeScene;
}

void Hierarchy::setActiveScene(EntityID rootEntityID) {
	if (std::find_if(scenes.begin(), scenes.end(), [&rootEntityID](const EntityHierarchyRootNode& data) { return data.entityID == rootEntityID; }) == scenes.end()) {
		std::cout << "Heirarchy::setActiveScene::ERROR Unable to set activeScene to " << rootEntityID << ". The scene does not exist in the Hierarchy." << std::endl;
		return;
	}
	this->activeScene = rootEntityID;
}

std::size_t Hierarchy::getSceneCount() {
	return scenes.size();
}