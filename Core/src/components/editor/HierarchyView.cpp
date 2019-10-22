#include "HierarchyView.h"
#include "components/RectTransform.h"
#include "components/graphics/ui/VerticalLayoutGroup.h"
#include "components/graphics/ui/HorizontalLayoutGroup.h"
#include "components/graphics/ui/LayoutElement.h"
#include "components/graphics/Text.h"
#include "components/graphics/ui/RectButton.h"
#include "components/graphics/RectSprite.h"
#include "HierarchySceneMover.h"
#include "HierarchyEntityMover.h"
#include "EditorPanel.h"
#include "input/Input.h"
#include "scene/SceneManager.h"
#include <algorithm>
using namespace Core;


HierarchyView::HierarchyView(ComponentHandle editor) : editor(editor) {}
HierarchyView::~HierarchyView() {}


void HierarchyView::onEnable() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		LayoutElement* element = owner.getComponent<LayoutElement>();
		element->setFlexibleSize(Vector2(1, 1));
		element->setFlexibleSizeEnabled(true);
		element->setMinSize(Vector2(0, 0));
		element->setMinSizeEnabled(true);

		VerticalLayoutGroup* group = owner.getComponent<VerticalLayoutGroup>();
		group->childForceExpandHeight = false;
		group->childForceExpandWidth = true;
		group->shrinkableChildHeight = false;
		group->shrinkableChildWidth = true;
		group->spacing = 5;
		group->paddingTop = 10;
		group->paddingLeft = 0;
		group->paddingRight = 0;
	}

	refresh();
	timer = refreshTime;
}

std::size_t HierarchyView::getOrder(EntityHandle& entity, EntityHandle& parent, std::vector<Handle>& rootEntities) const {
	if (parent.isValid()) {
		std::size_t childCount = parent.getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			if (parent.getChild(i) == entity) {
				return i;
			}
		}
	}
	else {
		std::size_t i = 0;
		for (Handle& root : rootEntities) {
			if (root == entity) return i;
			i++;
		}
	}
	return 0;
}

std::vector<HierarchyEntry> HierarchyView::getAllEntities() {
	std::vector<HierarchyEntry> entities;
	Scene* editorScene = owner.getScene();
	for (const ScenePtr& scene : sceneManager->getAllScenes()) {
		if (scene.get() == editorScene) continue;
		std::vector<Handle> rootEntities = scene->getRootEntities();
		for (EntityHandle entity : scene->getAllEntities()) {
			if (entity.getEntityHideFlags() == HideFlags::HideInHierarchy) continue;
			EntityHandle parent = entity.getParent();
			std::size_t order = getOrder(entity, parent, rootEntities);
			std::size_t depth = entity.getDepth();
			entities.push_back(HierarchyEntry(entity, parent, order, depth));
		}
	}
	return entities;
}

void HierarchyView::target(EntityHandle entity) {
	auto it = listMap.find(currentTarget.getEntity());
	if (it != listMap.end()) {
		it->second.highlight.getComponent<RectSprite>()->setColor(Color(0, 0, 0, 0));
	}
	currentTarget = entity;
	it = listMap.find(currentTarget.getEntity());
	if (it != listMap.end()) {
		it->second.highlight.getComponent<RectSprite>()->setColor(highlightColor);
	}
}

void HierarchyView::onTargetEntityClick(EntityHandle entity) {
	editor.getComponent<EditorPanel>()->selectTarget(entity);
}

void HierarchyView::onDestroyEntityClick(EntityHandle entity) {
	destroyEntity(entity);
}

void HierarchyView::clearList() {
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		destroyEntity(it->second);
	}
	sceneMap.clear();
	for (auto& entry : listMap) {
		destroyEntity(entry.second.entry);
	}
	listMap.clear();
	list.clear();
}

void HierarchyView::createEntityEntry(HierarchyEntry& entry, RectTransform* rect) {
	static const std::size_t ENTRY_HEIGHT = 20;
	EntityHandle& entity = entry.entity;
	EntityHandle parent = entry.parent.isValid() ? listMap[entry.parent.getEntity()].entry : sceneMap.at(entity.getScene()->getName());
	std::size_t depth = entry.depth + 1;

	std::string name = entity.getEntityName();
	std::string entityName = "Hierarchy_entry_" + name;
	Text text = Text(name, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255));
	std::size_t width = text.getSize().x + 12;

	EntityHandle entryHandle = createEntity(entityName,
		RectTransform(0, 0, width, 0, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	VerticalLayoutGroup* layoutGroup = entryHandle.addComponent<VerticalLayoutGroup>();
	layoutGroup->childForceExpandWidth = true;
	layoutGroup->childForceExpandHeight = false;
	layoutGroup->shrinkableChildWidth = true;
	layoutGroup->shrinkableChildHeight = false;
	layoutGroup->spacing = 5;

	RectButton button = RectButton();
	button.colors[RectButton::ButtonState::DEFAULT] = Color(255, 255, 255, 0);
	button.colors[RectButton::ButtonState::HOVER_OVER] = Color(255, 255, 255, 80);
	button.colors[RectButton::ButtonState::PRESSED_DOWN] = Color(0, 0, 0, 80);
	button.onLeftClick = Core::bind(this, &HierarchyView::onTargetEntityClick, entity);
	button.onRightClick = Core::bind(this, &HierarchyView::onDestroyEntityClick, entity);
	EntityHandle highlightHandle = createEntity(entityName + "_Background",
		RectSprite(Color(highlightColor.r, highlightColor.g, highlightColor.b, entity == currentTarget ? highlightColor.a : 0)),
		RectTransform(0, 0, 0, ENTRY_HEIGHT, rect->getZ() + 0.01f, Alignment::LEFT)
	);
	EntityHandle buttonEntity = createEntity(entityName + "_Button",
		button,
		RectSprite(),
		HierarchyEntityMover(ComponentHandle(this), entity),
		RectTransform((depth + 1) * 10, 0, width, ENTRY_HEIGHT, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	EntityHandle label = createEntity(entityName + "_Label",
		text,
		RectTransform(5, 0, width, text.getSize().y, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	highlightHandle.setParent(entryHandle);
	label.setParent(buttonEntity);
	buttonEntity.setParent(highlightHandle);
	entryHandle.setParent(parent);

	listMap[entity.getEntity()] = { entryHandle, highlightHandle };
	list.push_back(entry);
}

void HierarchyView::createSceneEntry(std::string name, Scene* scene, RectTransform* rect) {
	static const std::size_t ENTRY_HEIGHT = 20;
	EntityHandle parent = owner;
	static const std::size_t depth = 0;

	std::string entityName = "Hierarchy_entry_" + name;
	Text text = Text(name, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255));
	std::size_t width = text.getSize().x + 12;

	EntityHandle entryHandle = createEntity(entityName,
		RectTransform(0, 0, width, 0, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	VerticalLayoutGroup* layoutGroup = entryHandle.addComponent<VerticalLayoutGroup>();
	layoutGroup->childForceExpandWidth = true;
	layoutGroup->childForceExpandHeight = false;
	layoutGroup->shrinkableChildWidth = true;
	layoutGroup->shrinkableChildHeight = false;
	layoutGroup->spacing = 5;

	RectButton button = RectButton();
	button.colors[RectButton::ButtonState::DEFAULT] = Color(255, 255, 255, 0);
	button.colors[RectButton::ButtonState::HOVER_OVER] = Color(255, 255, 255, 80);
	button.colors[RectButton::ButtonState::PRESSED_DOWN] = Color(0, 0, 0, 80);
	EntityHandle highlightHandle = createEntity(entityName + "_Background",
		RectSprite(Color(highlightColor.r, highlightColor.g, highlightColor.b, false ? highlightColor.a : 0)),
		RectTransform(0, 0, 0, ENTRY_HEIGHT, rect->getZ() + 0.01f, Alignment::LEFT)
	);
	EntityHandle buttonEntity = createEntity(entityName + "_Button",
		button,
		RectSprite(),
		HierarchySceneMover(ComponentHandle(this), scene),
		RectTransform((depth + 1) * 10, 0, width, ENTRY_HEIGHT, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	EntityHandle label = createEntity(entityName + "_Label",
		text,
		RectTransform(5, 0, width, text.getSize().y, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	highlightHandle.setParent(entryHandle);
	label.setParent(buttonEntity);
	buttonEntity.setParent(highlightHandle);
	entryHandle.setParent(parent);

	sceneMap.insert(std::make_pair(name, entryHandle));
}

std::vector<HierarchyEntry>::iterator contains(std::vector<HierarchyEntry>& entities, const Entity& entity) {
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if (it->entity.getEntity() == entity) return it;
	}
	return entities.end();
}

std::vector<std::pair<std::string, ScenePtr>>::iterator contains(std::vector<std::pair<std::string, ScenePtr>>& scenes, const std::string& sceneName) {
	for (auto it = scenes.begin(); it != scenes.end(); it++) {
		if (it->first == sceneName) return it;
	}
	return scenes.end();
}

bool HierarchyView::isDirty(HierarchyEntry entry) {
	for (const HierarchyEntry& element : list) {
		if (element.entity == entry.entity) {
			if (element.depth != entry.depth || element.order != entry.order || element.parent != entry.parent) return true;
			if (entry.parent.isValid()) {
				auto it = contains(list, entry.parent.getEntity());
				if (it == list.end()) return true;
				return isDirty(*it);
			}
		}
	}
	return false;
}

void HierarchyView::refresh() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (!rect) return;
	// Scenes
	std::vector<std::pair<std::string, ScenePtr>> scenes = sceneManager->getAllScenesAsPairs();
	for (auto sceneIt = sceneMap.begin(); sceneIt != sceneMap.end();) {
		auto sceneIterator = contains(scenes, sceneIt->first);
		if (sceneIterator != scenes.end()) {
			scenes.erase(sceneIterator);
			sceneIt++;
		}
		else {
			EntityHandle& entry = sceneIt->second;
			for (auto it = list.begin(); it != list.end();) {
				if (it->entity.getScene()->getName() == sceneIt->first) {
					listMap.erase(it->entity.getEntity());
					list.erase(it);
				}
				else {
					it++;
				}
			}
			destroyEntity(entry);
		}
	}
	for (std::pair<std::string, ScenePtr>& scenePair : scenes) {
		createSceneEntry(scenePair.first, scenePair.second.get(), rect);
	}
	// Entities
	std::vector<HierarchyEntry> entities = getAllEntities();
	// Remove destroyed Entries
	auto it = list.begin();
	while (it != list.end()) {
		auto iterator = contains(entities, it->entity.getEntity());
		if (iterator != entities.end() && !isDirty(*iterator)) {
			it++;
			entities.erase(iterator);
		}
		else {
			destroyEntity(listMap[it->entity.getEntity()].entry);
			listMap.erase(it->entity.getEntity());
			list.erase(it);
		}
	}
	// Add new Entries
	std::sort(entities.begin(), entities.end(), [](const HierarchyEntry& lhs, const HierarchyEntry& rhs) {
		return lhs.depth < rhs.depth;
	});
	for (HierarchyEntry& newEntry : entities) {
		createEntityEntry(newEntry, rect);
	}
}

void HierarchyView::onDisable() {
	clearList();
}

void HierarchyView::lateUpdate(float deltaTime) {
	timer -= deltaTime;
	if (timer <= 0) {
		timer = refreshTime;
		refresh();
	}
}