#include "Hierarchy.h"
#include "utils/string.h"
#include "LevelEditor.h"
#include "EngineDLL.h"
#include "GameView.h"
#include "UndoRedo.h"
#include "utils/file.h"
#include <iostream>
#include <algorithm>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "IconsFontAwesome5.h"


constexpr wchar_t PREFAB_FILE_TYPE[] = L".prefab";

using namespace Editor;

void entitiesChangedCallback(void* ptr, std::size_t entityID) {
	static_cast<LevelEditor*>(ptr)->getHierarchy()->onEntitiesChanged(entityID);
}

bool revertPrefab(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager, Hierarchy* hierarchy, std::size_t rootEntityID, std::size_t entityID) {
	if (engineDLL->revertPrefab(entityID)) {
		hierarchy->setDirty(true);
		undoRedoManager->setDirty(rootEntityID, true);
		return true;
	}
	return false;
}

bool unpackPrefab(EngineDLL* engineDLL, UndoRedoManager* undoRedoManager, Hierarchy* hierarchy, std::size_t rootEntityID, std::size_t entityID) {
	auto propertyOverrides = engineDLL->getPropertyOverridesAt(entityID);
	auto prefabTypeID = engineDLL->getPrefabComponentTypeID();
	auto serializedComponentData = engineDLL->writeComponentToBuffer(entityID, prefabTypeID);
	if (engineDLL->unpackPrefab(entityID)) {
		hierarchy->setDirty(true);
		undoRedoManager->registerUndo(std::make_unique<RepackPrefabAction>(rootEntityID, entityID, std::move(serializedComponentData), propertyOverrides));
		return true;
	}
	return false;
}

void entityNode(LevelEditor* editor, Hierarchy* hierarchy, EngineDLL* engineDLL, UndoRedoManager* undoRedoManager, std::size_t rootEntityID, EntityHierarchyNode& node, EntityID targetID, GameView* gameView) {
	std::string name = node.name + "(" + std::to_string(node.entityID) + ")";
	std::string label = (node.isPrefabRoot ? ICON_FA_CUBE : ICON_FA_MALE) + (" " + name);
	std::size_t childCount = node.children.size();
	bool selected = node.entityID == targetID;
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
	if (selected) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if (childCount == 0) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	bool openPopup = false;
	bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), flags, label.c_str());
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		gameView->setTarget(node.entityID);
	}
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		openPopup = true;
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
			entityNode(editor, hierarchy, engineDLL, undoRedoManager, rootEntityID, child, targetID, gameView);
		}
		ImGui::TreePop();
	}
	std::string scenePopupID = "entity_hierarchy_popup_" + std::to_string(node.entityID);
	if (openPopup) {
		ImGui::OpenPopup(scenePopupID.c_str());
	}
	if (ImGui::BeginPopup(scenePopupID.c_str())) {
		if (ImGui::Selectable("New Entity", false)) {
			editor->getPopupManager()->openCreateEntity(node.entityID);
		}
		if (!node.isPrefabRoot) {
			if (ImGui::Selectable("Convert to Prefab", false)) {
				ProjectSettings& projectSettings = *editor->getProjectSettings();
				std::wstring filter = L"Prefab\0*" + std::wstring(PREFAB_FILE_TYPE) + L";\0";
				std::wstring path = getSaveFileName(L"Choose prefab location", filter.c_str(), 1, projectSettings.getPath().c_str());
				if (!path.empty()) {
					if (!path.ends_with(PREFAB_FILE_TYPE)) {
						path.append(PREFAB_FILE_TYPE);
					}
					std::string encodedPath = utf8_encode(path);
					if (engineDLL->createPrefabFromEntity(node.entityID, encodedPath.c_str())) {
						undoRedoManager->registerUndo(std::make_unique<UnpackPrefabAction>(rootEntityID, node.entityID));
						hierarchy->setDirty(true);
					}
					else {
						std::cout << "Failed to create prefab from Entity" << std::endl;
					}
				}
			}
		}
		else {
			if (ImGui::Selectable("Apply all changes to Prefab", false)) { // TODO: Confirm popup
				std::string path = engineDLL->getPrefabFilePath(node.entityID);
				if (path.empty()) {
					std::cout << "Failed to save prefab since the given Entity does not have a path" << std::endl;
				} else if (engineDLL->savePrefab(node.entityID, path.c_str())) {
					engineDLL->updatePrefabs(path.c_str());
					hierarchy->setDirty(true);
				}
				else {
					std::cout << "Failed to save prefab to path: " << path << std::endl;
				}
			}
			if (ImGui::Selectable("Revert all changes to Prefab", false)) { // TODO: Confirm popup
				if (!revertPrefab(engineDLL, undoRedoManager, hierarchy, rootEntityID, node.entityID)) {
					std::cout << "Failed to revert prefab for entity with ID " << node.entityID << std::endl;
				}
			}
			if (ImGui::Selectable("Unpack Prefab", false)) { // TODO: Confirm popup
				if (!unpackPrefab(engineDLL, undoRedoManager, hierarchy, rootEntityID, node.entityID)) {
					std::cout << "Failed to unpack prefab for entity with ID " << node.entityID << std::endl;
				}
			}
			if (ImGui::Selectable("Update all connected Prefabs", false)) {
				std::string path = engineDLL->getPrefabFilePath(node.entityID);
				if (path.empty()) {
					std::cout << "Failed to update prefab since the given Entity does not have a path" << std::endl;
				} else if (engineDLL->updatePrefabs(path.c_str())) {
					hierarchy->setDirty(true);
				}
				else {
					std::cout << "Failed to update prefabs at path: " << path << std::endl;
				}
			}
		}
		// TODO: Destroy
		ImGui::EndPopup();
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
		std::string sceneLabel = (scene.isPrefabRoot ? ICON_FA_CUBES : ICON_FA_MALE) + (" " + sceneName);
		if (undoRedoManager->getStepsSinceSave(rootEntityID) != 0) {
			sceneLabel.append("*");
		}
		std::string scenePopupId = std::string("scene_popup_").append(sceneName).append(std::to_string(rootEntityID));
		bool nodeOpened = ImGui::TreeNodeEx(sceneLabel.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | (activeScene == rootEntityID ? ImGuiTreeNodeFlags_Selected : 0));
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			openPopup = true;
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY")) {
				IM_ASSERT(payload->DataSize == sizeof(EntityID));
				EntityID* data = static_cast<EntityID*>(payload->Data);
				EntityID& entityID = *data;
				EntityID prevParentID = engineDLL->getEntityParent(entityID);
				if (engineDLL->setEntityParent(entityID, scene.entityID)) {
					undoRedoManager->registerUndo(std::make_unique<SetEntityParentAction>(rootEntityID, entityID, prevParentID));
				}
			}
			ImGui::EndDragDropTarget();
		}
		if (nodeOpened) {
			for (EntityHierarchyNode& entity : scene.children) {
				entityNode(editor, this, engineDLL, undoRedoManager, rootEntityID, entity, target, gameView);
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
			if (scene.isPrefabRoot) {
				if (ImGui::Selectable("Save", false)) {
					if (editor->isInEditMode() && engineDLL->isLoaded()) {
						engineDLL->savePrefab(scene.entityID, scene.filePath.c_str());
						engineDLL->updatePrefabs(scene.filePath.c_str());
						undoRedoManager->setDirty(rootEntityID, false);
						undoRedoManager->resetStepsSinceSave(rootEntityID);
					}
				}
				if (ImGui::Selectable("Revert all changes to Prefab", false)) { // TODO: Confirm popup
					if (!revertPrefab(engineDLL, undoRedoManager, this, rootEntityID, scene.entityID)) {
						std::cout << "Failed to revert prefab for entity with ID " << scene.entityID << std::endl;
					}
				}
				if (ImGui::Selectable("Unpack Prefab", false)) { // TODO: Confirm popup
					if (!unpackPrefab(engineDLL, undoRedoManager, this, rootEntityID, scene.entityID)) {
						std::cout << "Failed to unpack prefab for entity with ID " << scene.entityID << std::endl;
					}
				}
				if (ImGui::Selectable("Update all connected Prefabs", false)) {
					std::string path = engineDLL->getPrefabFilePath(scene.entityID);
					if (path.empty()) {
						std::cout << "Failed to update prefab since the given Entity does not have a path" << std::endl;
					}
					else if (engineDLL->updatePrefabs(path.c_str())) {
						setDirty(true);
					}
					else {
						std::cout << "Failed to update prefabs at path: " << path << std::endl;
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
			} else {
				if (ImGui::Selectable("Convert to Prefab", false)) {
					ProjectSettings& projectSettings = *editor->getProjectSettings();
					std::wstring filter = L"Prefab\0*" + std::wstring(PREFAB_FILE_TYPE) + L";\0";
					std::wstring path = getSaveFileName(L"Choose prefab location", filter.c_str(), 1, projectSettings.getPath().c_str());
					if (!path.empty()) {
						if (!path.ends_with(PREFAB_FILE_TYPE)) {
							path.append(PREFAB_FILE_TYPE);
						}
						std::string encodedPath = utf8_encode(path);
						if (engineDLL->createPrefabFromEntity(scene.entityID, encodedPath.c_str())) {
							undoRedoManager->registerUndo(std::make_unique<UnpackPrefabAction>(rootEntityID, rootEntityID));
							setDirty(true);
						}
						else {
							std::cout << "Failed to create prefab from Entity" << std::endl;
						}
					}
				}
			}
			// TODO: Close without save. Show confirmation popup
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

EntityHierarchyNode createEntityHierarchyFrom(EngineDLL* engineDLL, EntityID entityID) {
	EntityHierarchyNode node;
	node.entityID = entityID;
	node.isPrefabRoot = engineDLL->isEntityPrefabRoot(entityID);
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
			data.isPrefabRoot = engineDLL->isEntityPrefabRoot(entityID);
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

std::size_t Hierarchy::getSceneCount() const {
	return scenes.size();
}

std::vector<EntityID> Hierarchy::getSceneRootIDs() const {
	std::vector<EntityID> rootEntityIDs(scenes.size());
	for (std::size_t i = 0; i < rootEntityIDs.size(); i++) {
		rootEntityIDs[i] = scenes[i].entityID;
	}
	return rootEntityIDs;
}

std::vector<std::string> Hierarchy::getSceneNames() const {
	std::vector<std::string> names(scenes.size());
	for (std::size_t i = 0; i < names.size(); i++) {
		names[i] = scenes[i].name;
	}
	return names;
}