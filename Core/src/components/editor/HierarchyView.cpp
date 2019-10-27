#include "HierarchyView.h"
#include "components/RectTransform.h"
#include "components/graphics/ui/VerticalLayoutGroup.h"
#include "components/graphics/ui/HorizontalLayoutGroup.h"
#include "components/graphics/ui/LayoutElement.h"
#include "components/graphics/Text.h"
#include "components/graphics/ui/RectButton.h"
#include "components/graphics/RectSprite.h"
#include "components/entity/ChildManager.h"
#include "HierarchySceneMover.h"
#include "HierarchyEntityMover.h"
#include "HierarchyOrderRect.h"
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
		group->spacing = 0;
		group->paddingTop = 10;
		group->paddingLeft = 0;
		group->paddingRight = 0;
	}

	refresh();
	timer = refreshTime;
}

std::size_t HierarchyView::getRootIndex(const Handle& handle) {
	auto it = rootMap.find(handle.getScene()->getName());
	if (it == rootMap.end()) {
		std::cout << "HierarchyView::getRootIndex::ERROR The Scene does not exist in the rootMap.";
		return 0;
	}
	std::vector<Entity>& roots = it->second;
	auto entityIt = std::find(roots.begin(), roots.end(), handle.getEntity());
	if (entityIt == roots.end()) {
		//std::cout << "HierarchyView::setRootIndex::ERROR The Entity has not been given a root index.";
		roots.push_back(handle.getEntity());
		return roots.size() - 1;
	}
	return entityIt - roots.begin();
}

void HierarchyView::setRootIndex(const Handle& handle, std::size_t index) {
	auto it = rootMap.find(handle.getScene()->getName());
	if (it == rootMap.end()) return;
	std::vector<Entity>& roots = it->second;
	auto entityIt = std::find(roots.begin(), roots.end(), handle.getEntity());
	if (entityIt == roots.end()) {
		std::cout << "HierarchyView::setRootIndex::ERROR The Entity has not been given a root index.";
		return;
	}
	auto eraseIt = roots.erase(entityIt);
	if (index > eraseIt - roots.begin()) index--;
	if (index >= roots.size()) index = roots.size();
	roots.insert(roots.begin() + index, handle.getEntity());
}

std::vector<HierarchyEntry> HierarchyView::getAllEntities() {
	std::vector<HierarchyEntry> entities;
	Scene* editorScene = owner.getScene();
	for (const ScenePtr& scene : sceneManager->getAllScenes()) {
		if (scene.get() == editorScene) continue;
		for (EntityHandle entity : scene->getAllEntities()) {
			if (entity.getEntityHideFlags() == HideFlags::HideInHierarchy) continue;
			EntityHandle parent = entity.getParent();
			std::size_t order = entity.hasParent() ? entity.getSiblingIndex() : getRootIndex(entity); // TODO: Different for root objects
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

EntityHandle HierarchyView::createOrderRect(std::string name, EntityHandle entity, std::size_t order, RectTransform* rect) {
	return createEntity(name + "_Order_Rect",
		HierarchyOrderRect(entity, order),
		RectSprite(Color(0, 0, 0, 10)),
		RectTransform(0, 20, 0, 10, rect->getZ() + 0.3f, Alignment::CENTER)
	);
}

void HierarchyView::createEntityEntry(HierarchyEntry& entry, RectTransform* rect) {
	static const std::size_t ENTRY_HEIGHT = 15;
	EntityHandle& entity = entry.entity;
	EntityHandle parent = entry.parent.isValid() ? listMap[entry.parent.getEntity()].entry : sceneMap.at(entity.getScene()->getName());
	std::size_t depth = entry.depth + 1;
	std::size_t& order = entry.order;

	std::string name = entity.getEntityName();
	std::string entityName = "Hierarchy_entry_" + name;
	Text text = Text(name + "_" + std::to_string(order), "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255));
	std::size_t width = text.getSize().x + 12;

	VerticalLayoutGroup layoutGroup = VerticalLayoutGroup();
	layoutGroup.childForceExpandWidth = true;
	layoutGroup.childForceExpandHeight = false;
	layoutGroup.shrinkableChildWidth = true;
	layoutGroup.shrinkableChildHeight = false;
	layoutGroup.spacing = 0;
	EntityHandle entryHandle = createEntity(entityName,
		layoutGroup,
		RectTransform(0, 0, width, 0, rect->getZ() + 0.1f, Alignment::LEFT)
	);

	RectButton button = RectButton();
	button.colors[RectButton::ButtonState::DEFAULT] = Color(255, 255, 255, 0);
	button.colors[RectButton::ButtonState::HOVER_OVER] = Color(255, 255, 255, 80);
	button.colors[RectButton::ButtonState::PRESSED_DOWN] = Color(0, 0, 0, 80);
	button.onLeftClick = Core::bind(this, &HierarchyView::onTargetEntityClick, entity);
	button.onRightClick = Core::bind(this, &HierarchyView::onDestroyEntityClick, entity);
	EntityHandle highlightHandle = createEntity(entityName + "_Background",
		layoutGroup,
		RectSprite(Color(highlightColor.r, highlightColor.g, highlightColor.b, entity == currentTarget ? highlightColor.a : 0)),
		RectTransform(0, 0, 0, ENTRY_HEIGHT, rect->getZ() + 0.01f, Alignment::LEFT)
	);
	EntityHandle buttonEntity = createEntity(entityName + "_Button",
		button,
		RectSprite(),
		HierarchyEntityMover(ComponentHandle(this), entity),
		RectTransform(0, 0, 0, ENTRY_HEIGHT, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	EntityHandle label = createEntity(entityName + "_Label",
		text,
		RectTransform((depth) * 20, 0, width, text.getSize().y, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	highlightHandle.setParent(entryHandle);
	label.setParent(buttonEntity);
	buttonEntity.setParent(highlightHandle);
	entryHandle.setParent(parent);

	// Order Rects
	//createOrderRect(entityName, entry.parent, order + 1, rect).setParent(entryHandle);
	createOrderRect(entityName, entry.entity, order, rect).setParent(entryHandle);

	listMap[entity.getEntity()] = { entryHandle, highlightHandle };
	std::size_t index = 0;

	// Sort new entry
	auto it = list.begin();
	while (it != list.end()) {
		const HierarchyEntry& rhs = *it;
		if (depth < rhs.depth || depth == rhs.depth && order < rhs.order) break;
		it++;
	}
	list.insert(it, entry);
}

void HierarchyView::createSceneEntry(std::string name, Scene* scene, RectTransform* rect) {
	static const std::size_t ENTRY_HEIGHT = 20;
	static const std::size_t depth = 0;
	std::size_t order = sceneMap.size();

	std::string entityName = "Hierarchy_entry_" + name;
	Text text = Text(name, "resources/fonts/segoeui.ttf", 15, Color(0, 0, 0));
	std::size_t width = text.getSize().x + 12;

	VerticalLayoutGroup layoutGroup = VerticalLayoutGroup();
	layoutGroup.childForceExpandWidth = true;
	layoutGroup.childForceExpandHeight = false;
	layoutGroup.shrinkableChildWidth = true;
	layoutGroup.shrinkableChildHeight = false;
	layoutGroup.spacing = 0;
	EntityHandle entryHandle = createEntity(entityName,
		layoutGroup,
		RectTransform(0, 0, width, 0, rect->getZ() + 0.1f, Alignment::LEFT)
	);

	EntityHandle header = createEntity(entityName + "_Header",
		HierarchySceneMover(ComponentHandle(this), scene),
		RectSprite(Color(200, 200, 200, 255)),
		RectTransform(0, 0, width, ENTRY_HEIGHT, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	EntityHandle label = createEntity(entityName + "_Label",
		text,
		RectTransform(5, 0, width, text.getSize().y, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	header.setParent(entryHandle);
	label.setParent(header);
	entryHandle.setParent(owner);
	// Order Rect
	createOrderRect(entityName, EntityHandle(), 0, rect).setParent(entryHandle);

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
	// Remove destroyed Scenes
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
			auto rootMapIt = rootMap.find(sceneIt->first);
			rootMap.erase(rootMapIt);
		}
	}
	// Add new scenes
	for (std::pair<std::string, ScenePtr>& scenePair : scenes) {
		createSceneEntry(scenePair.first, scenePair.second.get(), rect);
		rootMap.insert(std::make_pair(scenePair.first, std::vector<Entity>{}));
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
			if (it->depth == 0 && iterator->depth != 0) { // Check if root converting to child
				auto rootMapIt = rootMap.find(it->entity.getScene()->getName());
				if (rootMapIt != rootMap.end()) {
					std::vector<Entity>& roots = rootMapIt->second;
					auto rootIt = std::find(roots.begin(), roots.end(), it->entity.getEntity());
					if (rootIt != roots.end()) {
						roots.erase(rootIt);
					}
					else {
						std::cout << "HierarchyView::refresh::ERROR The Entity did not have a root order.";
					}
				}
				else {
					std::cout << "HierarchyView::refresh::ERROR The Scene does not exist in the rootMap.";
				}
			}
			destroyEntity(listMap[it->entity.getEntity()].entry);
			listMap.erase(it->entity.getEntity());
			list.erase(it);
		}
	}
	// Add new Entries
	std::sort(entities.begin(), entities.end(), [](const HierarchyEntry& lhs, const HierarchyEntry& rhs) {
		if (lhs.depth < rhs.depth) return true;
		if (lhs.depth > rhs.depth) return false;
		return lhs.order < rhs.order;
	});
	for (HierarchyEntry& newEntry : entities) {
		createEntityEntry(newEntry, rect);
		// Updating dirty order map
		Entity parent(0);
		if (newEntry.parent.isValid())
			parent = newEntry.parent.getEntity();

		auto it = dirtyOrderMap.find(parent);
		if (it != dirtyOrderMap.end()) {
			if (it->second > newEntry.order)
				it->second = newEntry.order;
		}
		else {
			dirtyOrderMap.insert(std::make_pair(parent, newEntry.order));
		}
	}
}

void HierarchyView::onDisable() {
	clearList();
}

void HierarchyView::updateOrder() {
	for (auto orderIt = dirtyOrderMap.begin(); orderIt != dirtyOrderMap.end(); orderIt++) {
		// Update order
		const Entity& entity = orderIt->first;
		std::size_t& order = orderIt->second;

		for (HierarchyEntry& entry : list) {
			EntityHandle entityEntry = getEntryHandle(entry.entity.getEntity());
			if (!entityEntry.isValid())
				std::cout << "HierarchyView::UpdateOrder::ERROR Entity does not have any Entry" << std::endl;
			else
				entityEntry.setSiblingIndex(entry.order + 2);
		}

		// Force update layout
		auto listMapIt = listMap.find(entity);
		if (listMapIt != listMap.end()) {
			if (LayoutGroup* layout = listMapIt->second.entry.getComponent<LayoutGroup>()) {
				layout->setDirty();
			}
		}
	}
	dirtyOrderMap.clear();
}

void HierarchyView::lateUpdate(float deltaTime) {
	updateOrder();
	timer -= deltaTime;
	if (timer <= 0) {
		timer = refreshTime;
		refresh();
	}
}

EntityHandle HierarchyView::getEntryHandle(const Entity& entity) {
	auto entryIt = listMap.find(entity);
	if (entryIt == listMap.end())
		return EntityHandle();
	return entryIt->second.entry;
}